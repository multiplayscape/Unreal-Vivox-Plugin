/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxAudioDevice.h"

#include "IAudioDevice.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxAudioDevice *UVivoxAudioDevice::CreateVivoxAudioDevice(IAudioDevice *VoiceAudioDevice) {
	UVivoxAudioDevice *vivoxAudioDevice = NewObject<UVivoxAudioDevice>();
	vivoxAudioDevice->VivoxVoiceAudioDevice = VoiceAudioDevice;

	return vivoxAudioDevice;
}

void UVivoxAudioDevice::BeginDestroy() {

	VivoxVoiceAudioDevice = nullptr;

	this->UObject::BeginDestroy();

}

FString UVivoxAudioDevice::Name() const {
	return VivoxVoiceAudioDevice->Name();
}

FString UVivoxAudioDevice::Id() const {
    return VivoxVoiceAudioDevice->Id();
}

bool UVivoxAudioDevice::IsEmpty() const {
    return VivoxVoiceAudioDevice->IsEmpty();
}