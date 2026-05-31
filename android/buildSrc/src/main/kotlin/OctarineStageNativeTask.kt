import org.gradle.api.DefaultTask
import org.gradle.api.file.DirectoryProperty
import org.gradle.api.file.RegularFileProperty
import org.gradle.api.provider.Property
import org.gradle.api.tasks.Input
import org.gradle.api.tasks.InputFile
import org.gradle.api.tasks.OutputDirectory
import org.gradle.api.tasks.PathSensitive
import org.gradle.api.tasks.PathSensitivity
import org.gradle.api.tasks.TaskAction

/**
 * Stages a freshly-built libmain.so for one Android ABI into an AGP-shaped jniLibs source
 * directory: `<outputDirectory>/<abi>/libmain.so`. The output directory is exposed back to AGP
 * via `variant.sources.jniLibs.addGeneratedSourceDirectory(stageTask, OctarineStageNativeTask::outputDirectory)`,
 * which wires the dependency edge (assembleX depends on stage) and the packaging step (the .so
 * lands at lib/<abi>/libmain.so inside the APK/AAB) without any preBuild.dependsOn glue.
 */
abstract class OctarineStageNativeTask : DefaultTask() {
    @get:InputFile
    @get:PathSensitive(PathSensitivity.RELATIVE)
    abstract val libmain: RegularFileProperty

    @get:Input
    abstract val abi: Property<String>

    @get:OutputDirectory
    abstract val outputDirectory: DirectoryProperty

    @TaskAction
    fun stage() {
        val abiDir = outputDirectory.get().asFile.resolve(abi.get())
        abiDir.mkdirs()
        libmain.get().asFile.copyTo(abiDir.resolve("libmain.so"), overwrite = true)
    }
}
