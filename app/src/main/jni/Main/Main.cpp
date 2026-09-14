//
// Created by reveny on 21/08/2023.
//

#include "../Include/KittyMemory/MemoryPatch.h"
#include "../Include/ImGui.h"
#include "../Include/RemapTools.h"

#include "../Include/Drawing.h"
#include "../Include/Unity.h"

bool aimbotEnabled = false;
bool espEnabled = false;
float fovValue = 90.0f;

void DrawMenu() {
    ImGui::Begin("Meu Mod Menu | Android", nullptr, ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("Status: Ativo");
    ImGui::Separator();
    ImGui::Checkbox("Aimbot", &aimbotEnabled);
    ImGui::Checkbox("ESP Box", &espEnabled);
    ImGui::SliderFloat("FOV do Aimbot", &fovValue, 10.0f, 180.0f);
    ImGui::End();
}


void *thread(void *) {
    LOGI(OBFUSCATE("Main Thread Loaded: %d"), gettid());
    initModMenu((void *)DrawMenu);

    //Hooks, Patches and Pointers here
    //Example:
    /*
     * DobbyHook(getAbsoluteAddress("libIl2cpp.so", 0x0), FunctionExample, old_FunctionExample);
     * SetAimRotation = (void (*)(void *, Quaternion)) getAbsoluteAddress("libIl2cpp.so", 0x0);
     */

    LOGI("Main thread done");
    pthread_exit(0);
}

// Call anything from JNI_OnLoad here
extern "C" {
    // JNI Support
    JavaVM *jvm = nullptr;
    JNIEnv *env = nullptr;

    __attribute__((visibility ("default")))
    jint loadJNI(JavaVM *vm) {
        jvm = vm;
        vm->AttachCurrentThread(&env, nullptr);
        LOGI("loadJNI(): Initialized");

        return JNI_VERSION_1_6;
    }
}

__attribute__((constructor))
void init() {
    LOGI("Loaded Mod Menu");

    pthread_t t;
    pthread_create(&t, nullptr, thread, nullptr);

    //Don't leave any traces, remap the loader lib as well
    RemapTools::RemapLibrary("libLoader.so");
}
