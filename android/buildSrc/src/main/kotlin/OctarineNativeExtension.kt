import org.gradle.api.provider.ListProperty
import org.gradle.api.provider.MapProperty
import org.gradle.api.provider.Property

/**
 * Plugin-facing configuration for octarine-native. Kept minimal — every value is something the
 * caller already had to compute (vcpkgRoot, ndkHome, repoRoot) or is intrinsic to the engine
 * (overlay ports dir, minSdk, abi→triplet mapping).
 *
 * `abis` is the multi-ABI fan-out trigger: when empty (default), the plugin is a no-op and the
 * app build's `if (!multiAbi) externalNativeBuild { ... }` path handles single-ABI native compile.
 * Pass `-Poctarine.abis=arm64-v8a,x86_64` upstream and the build script forwards the resolved list
 * into this property.
 */
interface OctarineNativeExtension {
    val abis: ListProperty<String>
    val vcpkgRoot: Property<String>
    val ndkHome: Property<String>
    val repoRoot: Property<String>
    val vcpkgOverlayPorts: Property<String>
    val minSdk: Property<Int>
    val tripletForAbi: MapProperty<String, String>
}
