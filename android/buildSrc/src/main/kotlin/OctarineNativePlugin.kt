import com.android.build.api.variant.ApplicationAndroidComponentsExtension
import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.api.tasks.Exec
import org.gradle.kotlin.dsl.create
import org.gradle.kotlin.dsl.getByType
import org.gradle.kotlin.dsl.register
import java.io.File

/**
 * Multi-ABI native build orchestration for the Android host app. Replaces the bespoke
 * `octarineNative_<abi>_{configure,build,stage}` task fan-out that used to live inline in
 * `android/app/build.gradle` with a `Plugin<Project>` + a typed stage task, and uses AGP's
 * `androidComponents.onVariants` to wire the .so outputs into each variant's jniLibs rather than
 * the previous `tasks.configureEach { if (task.name == 'preBuild') task.dependsOn ... }` shim.
 *
 * Single-ABI mode (no `octarineNative { abis = [...] }` configured): the plugin is a no-op and
 * AGP's externalNativeBuild path drives the native compile (faster IDE inner loop).
 *
 * Multi-ABI mode: for every ABI in `extension.abis`, three tasks are registered:
 *   octarineNative_<abi>_configure   — cmake configure into build/octarineJni/.build/<abi>
 *   octarineNative_<abi>_build       — cmake --build (produces libmain.so)
 *   octarineNative_<abi>_stage       — copy libmain.so to build/octarineJni/<abi>/<abi>/libmain.so
 * The stage task's outputDirectory is attached per variant via
 * `variant.sources.jniLibs.addGeneratedSourceDirectory(...)` — AGP then derives the dependency
 * edge automatically and packages the .so at `lib/<abi>/libmain.so` in the APK/AAB.
 */
class OctarineNativePlugin : Plugin<Project> {
    override fun apply(project: Project) {
        val ext = project.extensions.create<OctarineNativeExtension>("octarineNative")
        ext.minSdk.convention(28)
        ext.tripletForAbi.convention(
            mapOf(
                "arm64-v8a" to "arm64-android",
                "x86_64" to "x64-android",
                "armeabi-v7a" to "arm-android",
                "x86" to "x86-android",
            )
        )

        val androidComponents = project.extensions.getByType<ApplicationAndroidComponentsExtension>()

        // onVariants fires once per build variant (debug + release). The per-ABI Exec/stage
        // tasks are content-identical across variants, so register them lazily on first sight and
        // reuse the existing TaskProvider for the second variant.
        androidComponents.onVariants { variant ->
            val abis = ext.abis.get()
            if (abis.isEmpty()) return@onVariants

            abis.forEach { abi ->
                val triplet = ext.tripletForAbi.get()[abi] ?: "arm64-android"
                val stageTask = registerAbiTasks(project, ext, abi, triplet)
                variant.sources.jniLibs?.addGeneratedSourceDirectory(
                    stageTask,
                    OctarineStageNativeTask::outputDirectory,
                )
            }
        }
    }

    private fun registerAbiTasks(
        project: Project,
        ext: OctarineNativeExtension,
        abiName: String,
        triplet: String,
    ): org.gradle.api.tasks.TaskProvider<OctarineStageNativeTask> {
        val nativeRoot = project.layout.buildDirectory.dir("octarineJni").get().asFile
        val configureDir = File(nativeRoot, ".build/$abiName")
        val stageDir = File(nativeRoot, ".staged/$abiName")

        val configureTaskName = "octarineNative_${abiName}_configure"
        val buildTaskName = "octarineNative_${abiName}_build"
        val stageTaskName = "octarineNative_${abiName}_stage"

        // Bail out early if this ABI's tasks are already registered (second variant pass).
        project.tasks.findByName(stageTaskName)?.let {
            @Suppress("UNCHECKED_CAST")
            return project.tasks.named(stageTaskName) as org.gradle.api.tasks.TaskProvider<OctarineStageNativeTask>
        }

        val configureTask = project.tasks.register<Exec>(configureTaskName) {
            description = "CMake configure for Android ABI $abiName (triplet $triplet)."
            doFirst {
                configureDir.mkdirs()
            }
            workingDir = configureDir
            environment("ANDROID_NDK_HOME", ext.ndkHome.get())
            val ndkToolchain = File(ext.ndkHome.get(), "build/cmake/android.toolchain.cmake")
            commandLine(
                "cmake",
                ext.repoRoot.get(),
                "-G", "Ninja",
                "-DANDROID_ABI=$abiName",
                "-DANDROID_PLATFORM=android-${ext.minSdk.get()}",
                "-DANDROID_STL=c++_shared",
                "-DCMAKE_TOOLCHAIN_FILE=${ext.vcpkgRoot.get()}/scripts/buildsystems/vcpkg.cmake",
                "-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=$ndkToolchain",
                "-DVCPKG_TARGET_TRIPLET=$triplet",
                "-DVCPKG_MANIFEST_MODE=ON",
                "-DVCPKG_OVERLAY_PORTS=${ext.vcpkgOverlayPorts.get()}",
                "-DCMAKE_BUILD_TYPE=Release",
            )
        }

        val buildTask = project.tasks.register<Exec>(buildTaskName) {
            description = "CMake build of libmain.so for Android ABI $abiName."
            dependsOn(configureTask)
            workingDir = configureDir
            commandLine("cmake", "--build", ".", "--target", "OctarineEngine", "--parallel")
        }

        return project.tasks.register<OctarineStageNativeTask>(stageTaskName) {
            description = "Stage libmain.so for Android ABI $abiName into a jniLibs-shaped directory."
            dependsOn(buildTask)
            abi.set(abiName)
            libmain.set(project.layout.file(project.provider { File(configureDir, "libmain.so") }))
            outputDirectory.set(stageDir)
        }
    }
}
