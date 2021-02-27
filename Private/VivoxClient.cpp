/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#include "VivoxClient.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxClient::UVivoxClient(const FObjectInitializer &ObjectInitializer) : UObject(ObjectInitializer) {
    LOG_VIVOX_NORMAL(FString::Printf(TEXT("Entered ::UVivoxClient")));

    VivoxVoiceClient = &static_cast<FVivoxCoreModule *>(&FModuleManager::Get().LoadModuleChecked(
            TEXT("VivoxCore")))->VoiceClient();
}

UVivoxClient *UVivoxClient::GetVivoxClient() {
    UVivoxClient *vivoxClient = NewObject<UVivoxClient>();
    return vivoxClient;
}

void UVivoxClient::BeginDestroy() {
	VivoxVoiceClient = nullptr;
	this->UObject::BeginDestroy();
}

EVivoxStatus UVivoxClient::Initialize(const FVivoxClientConfig& clientConfig) {
    LOG_VIVOX_NORMAL(FString::Printf(TEXT("Entered ::Initialize")));

    VivoxCoreError Status = this->VivoxVoiceClient->Initialize(clientConfig.toVivox());
    EVivoxStatus vivoxStatus = fromVivox(Status);
    ensure(vivoxStatus == EVivoxStatus::Success);
    if (vivoxStatus != EVivoxStatus::Success) {
        LOG_VIVOX_WARNING(
                FString::Printf(TEXT("Initialize failed: %s"), ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
    }

    return vivoxStatus;
}

void UVivoxClient::Uninitialize() {
    LOG_VIVOX_NORMAL(FString::Printf(TEXT("Entered ::Uninitialize")));

    this->VivoxVoiceClient->Uninitialize();
}

FString UVivoxClient::GetSDKVersion() {
    return IClient::InternalVersion();
}

FString UVivoxClient::GetSDKVersionExtended() {
    return IClient::InternalVersionExtended();
}

UVivoxLoginSession *UVivoxClient::GetLoginSession(const FVivoxAccountId &accountId) {
    return UVivoxLoginSession::CreateVivoxLoginSession(&(this->VivoxVoiceClient->GetLoginSession(accountId.toVivox())));
}

TMap<FVivoxAccountId, UVivoxLoginSession*> UVivoxClient::LoginSessions() {
	TMap<FVivoxAccountId, UVivoxLoginSession* > loginSessions;
	for (const TPair<AccountId, TSharedPtr<ILoginSession>>& loginSession : this->VivoxVoiceClient->LoginSessions()) {
		loginSessions.Add(FVivoxAccountId().fromVivox(loginSession.Key), UVivoxLoginSession::CreateVivoxLoginSession(loginSession.Value.Get()));
	}
	return loginSessions;
}

UVivoxAudioDevices *UVivoxClient::AudioInputDevices() {
    return UVivoxAudioDevices::CreateVivoxAudioDevices(&(this->VivoxVoiceClient->AudioInputDevices()));
}

UVivoxAudioDevices *UVivoxClient::AudioOutputDevices() {
    return UVivoxAudioDevices::CreateVivoxAudioDevices(&(this->VivoxVoiceClient->AudioOutputDevices()));
}