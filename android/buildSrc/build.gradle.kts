// buildSrc holds the repo-local Octarine gradle plugin (octarine-native). kotlin-dsl gives the
// plugin access to Kotlin + the AGP Variant API without a separate composite build. Gradle
// auto-applies whatever this directory builds before evaluating any other build script in the
// android/ project.

plugins {
    `kotlin-dsl`
}

repositories {
    google()
    mavenCentral()
    gradlePluginPortal()
}

// Pinned to the same AGP version the app applies (android/build.gradle). Bump in lockstep.
dependencies {
    implementation("com.android.tools.build:gradle:8.7.3")
}

gradlePlugin {
    plugins {
        create("octarineNative") {
            id = "octarine-native"
            implementationClass = "OctarineNativePlugin"
        }
    }
}
