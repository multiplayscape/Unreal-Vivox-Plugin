/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxChannelSession.h"

#include "IChannelSession.h"
#include "VivoxCore.h"

#include "VivoxChannelConnectionState.h"
#include "VivoxLoginSession.h"
#include "VivoxParticipant.h"
#include "VivoxChannelTextMessage.h"

#include "LatentActions.h"

#include "VivoxGlobals.h"

UVivoxChannelSession *UVivoxChannelSession::CreateVivoxChannelSession(IChannelSession *VoiceChannelSession) {
    UVivoxChannelSession *vivoxChannelSession = NewObject<UVivoxChannelSession>();
    vivoxChannelSession->VivoxVoiceChannelSession = VoiceChannelSession;

    {
        vivoxChannelSession->AudioStateChangedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventAudioStateChanged.AddLambda([vivoxChannelSession](const IChannelConnectionState & vivoxChannelConnectionState) {
            if (vivoxChannelSession->AudioStateChanged.IsBound()) {
                vivoxChannelSession->AudioStateChanged.Broadcast(UVivoxChannelConnectionState::CreateVivoxChannelConnectionState((IChannelConnectionState *)&vivoxChannelConnectionState));
            }
        });

        vivoxChannelSession->TextStateChangedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventTextStateChanged.AddLambda([vivoxChannelSession](const IChannelConnectionState & vivoxChannelConnectionState) {
            if (vivoxChannelSession->TextStateChanged.IsBound()) {
                vivoxChannelSession->TextStateChanged.Broadcast(UVivoxChannelConnectionState::CreateVivoxChannelConnectionState((IChannelConnectionState *)&vivoxChannelConnectionState));
            }
        });

        vivoxChannelSession->AfterParticipantAddedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventAfterParticipantAdded.AddLambda([vivoxChannelSession](const IParticipant & vivoxParticipant) {
            if (vivoxChannelSession->AfterParticipantAdded.IsBound()) {
                vivoxChannelSession->AfterParticipantAdded.Broadcast(UVivoxParticipant::CreateVivoxParticipant((IParticipant *)&vivoxParticipant));
            }
        });

        vivoxChannelSession->BeforeParticipantRemovedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventBeforeParticipantRemoved.AddLambda([vivoxChannelSession](const IParticipant & vivoxParticipant) {
            if (vivoxChannelSession->BeforeParticipantRemoved.IsBound()) {
                vivoxChannelSession->BeforeParticipantRemoved.Broadcast(UVivoxParticipant::CreateVivoxParticipant((IParticipant *)&vivoxParticipant));
            }
        });

        vivoxChannelSession->AfterParticipantUpdatedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventAfterParticipantUpdated.AddLambda([vivoxChannelSession](const IParticipant & vivoxParticipant) {
            if (vivoxChannelSession->AfterParticipantUpdated.IsBound()) {
                vivoxChannelSession->AfterParticipantUpdated.Broadcast(UVivoxParticipant::CreateVivoxParticipant((IParticipant *)&vivoxParticipant));
            }
        });

        vivoxChannelSession->TextMessageReceivedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventTextMessageReceived.AddLambda([vivoxChannelSession](const IChannelTextMessage & vivoxChannelTextMessage) {
            if (vivoxChannelSession->TextMessageReceived.IsBound()) {
                vivoxChannelSession->TextMessageReceived.Broadcast(UVivoxChannelTextMessage::CreateVivoxChannelTextMessage((IChannelTextMessage *)&vivoxChannelTextMessage));
            }
        });

        vivoxChannelSession->TranscribedMessageReceivedHandle = vivoxChannelSession->VivoxVoiceChannelSession->EventTranscribedMessageReceived.AddLambda([vivoxChannelSession](const ITranscribedMessage & vivoxTranscribedMessage) {
            if (vivoxChannelSession->TranscribedMessageReceived.IsBound()) {
                vivoxChannelSession->TranscribedMessageReceived.Broadcast(UVivoxTranscribedMessage::CreateVivoxTranscribedMessage((ITranscribedMessage *)&vivoxTranscribedMessage));
            }
        });
    }

    return vivoxChannelSession;
}

void UVivoxChannelSession::BeginDestroy() {
	this->AudioStateChanged.Clear();
	this->TextStateChanged.Clear();
	this->AfterParticipantAdded.Clear();
	this->BeforeParticipantRemoved.Clear();
	this->AfterParticipantUpdated.Clear();
	this->TextMessageReceived.Clear();
	this->TranscribedMessageReceived.Clear();

	if (this->VivoxVoiceChannelSession != nullptr) {
		this->VivoxVoiceChannelSession->EventAudioStateChanged.Remove(this->AudioStateChangedHandle);
		this->VivoxVoiceChannelSession->EventTextStateChanged.Remove(this->TextStateChangedHandle);
		this->VivoxVoiceChannelSession->EventAfterParticipantAdded.Remove(this->AfterParticipantAddedHandle);
		this->VivoxVoiceChannelSession->EventBeforeParticipantRemoved.Remove(this->BeforeParticipantRemovedHandle);
		this->VivoxVoiceChannelSession->EventAfterParticipantUpdated.Remove(this->AfterParticipantUpdatedHandle);
		this->VivoxVoiceChannelSession->EventTextMessageReceived.Remove(this->TextMessageReceivedHandle);
		this->VivoxVoiceChannelSession->EventTranscribedMessageReceived.Remove(this->TranscribedMessageReceivedHandle);
	}

	this->VivoxVoiceChannelSession = nullptr;
	this->UObject::BeginDestroy();
}

UVivoxLoginSession *UVivoxChannelSession::Parent() {
    return UVivoxLoginSession::CreateVivoxLoginSession(&(this->VivoxVoiceChannelSession->Parent()));
}

EVivoxConnectionState UVivoxChannelSession::AudioState() const {
    switch (this->VivoxVoiceChannelSession->AudioState()) {
        case ConnectionState::Connected:
            return EVivoxConnectionState::Connected;
            break;
        case ConnectionState::Disconnected:
            return EVivoxConnectionState::Disconnected;
            break;
        case ConnectionState::Connecting:
            return EVivoxConnectionState::Connecting;
            break;
        case ConnectionState::Disconnecting:
            return EVivoxConnectionState::Disconnecting;
            break;
        default:
			return EVivoxConnectionState::Disconnected;
            break;
    }
}

EVivoxConnectionState UVivoxChannelSession::TextState() const {
    switch (this->VivoxVoiceChannelSession->TextState()) {
        case ConnectionState::Connected:
            return EVivoxConnectionState::Connected;
            break;
        case ConnectionState::Disconnected:
            return EVivoxConnectionState::Disconnected;
            break;
        case ConnectionState::Connecting:
            return EVivoxConnectionState::Connecting;
            break;
        case ConnectionState::Disconnecting:
            return EVivoxConnectionState::Disconnecting;
            break;
        default:
			return EVivoxConnectionState::Disconnected;
            break;
    }
}

TMap<FString, UVivoxParticipant *> UVivoxChannelSession::Participants() const {
    TMap<FString, UVivoxParticipant *> participants;
    for (const TPair<FString, IParticipant *> participant : this->VivoxVoiceChannelSession->Participants()) {
        participants.Add(participant.Key, UVivoxParticipant::CreateVivoxParticipant(participant.Value));
    }
    return participants;
}

bool UVivoxChannelSession::Typing() const {
    return this->VivoxVoiceChannelSession->Typing();
}

void UVivoxChannelSession::SetTyping(bool value) {
    this->VivoxVoiceChannelSession->SetTyping(value);
}

bool UVivoxChannelSession::IsTransmitting() const {
	return this->VivoxVoiceChannelSession->IsTransmitting();
}


FVivoxChannelId UVivoxChannelSession::Channel() const {
    return FVivoxChannelId().fromVivox(this->VivoxVoiceChannelSession->Channel());
}

class FBeginConnectAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    IChannelSession *VivoxVoiceChannelSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginConnectAction(
            IChannelSession *vivoxVoiceChannelSession,
            bool connectAudio,
            bool connectText,
			bool switchTransmission,
            const FString &accessToken,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceChannelSession(vivoxVoiceChannelSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        IChannelSession::FOnBeginConnectCompletedDelegate OnBeginConnectCompleteCallback;
        OnBeginConnectCompleteCallback.BindLambda(
                [this, &status, vivoxVoiceChannelSession](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Connect failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                    }
                    completed = true;
                }
        );
        status = fromVivox(vivoxVoiceChannelSession->BeginConnect(connectAudio, connectText, switchTransmission, accessToken,
                                                                  OnBeginConnectCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return FString::Format(*NSLOCTEXT("Begin Connect Vivox", "channel domain", "%s").ToString(),
                               {VivoxVoiceChannelSession->Channel().Domain()});
    }

#endif
};

void UVivoxChannelSession::BeginConnect(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        bool connectAudio,
        bool connectText,
		bool switchTransmission,
        const FString &accessToken,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginConnectAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginConnectAction(this->VivoxVoiceChannelSession,
                                                                     connectAudio,
                                                                     connectText,
																	 switchTransmission,
                                                                     accessToken,
                                                                     status,
                                                                     LatentInfo));
        }
    }
}

void UVivoxChannelSession::Disconnect() {
    this->VivoxVoiceChannelSession->Disconnect();
}

class FBeginSetAudioConnectedAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    IChannelSession *VivoxVoiceChannelSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginSetAudioConnectedAction(
            IChannelSession *vivoxVoiceChannelSession,
            bool value,
			bool switchTransmission,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceChannelSession(vivoxVoiceChannelSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        IChannelSession::FOnBeginSetAudioConnectedCompletedDelegate OnBeginSetAudioConnectedCompleteCallback;
        OnBeginSetAudioConnectedCompleteCallback.BindLambda(
                [this, &status, vivoxVoiceChannelSession](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Set Audio Connected failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                    }
                    completed = true;
                }
        );
        status = fromVivox(vivoxVoiceChannelSession->BeginSetAudioConnected(value, switchTransmission,
                                                                            OnBeginSetAudioConnectedCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return FString::Format(*NSLOCTEXT("Begin Set Audio Connected", "channel domain", "%s").ToString(),
                               {VivoxVoiceChannelSession->Channel().Domain()});
    }

#endif
};

void UVivoxChannelSession::BeginSetAudioConnected(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        bool value,
		bool switchTransmission,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginSetAudioConnectedAction>(LatentInfo.CallbackTarget,
                                                                                  LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginSetAudioConnectedAction(this->VivoxVoiceChannelSession,
                                                                               value,
																				switchTransmission,
                                                                               status,
                                                                               LatentInfo));
        }
    }
}

class FBeginSetTextConnectedAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    IChannelSession *VivoxVoiceChannelSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginSetTextConnectedAction(
            IChannelSession *vivoxVoiceChannelSession,
            bool value,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceChannelSession(vivoxVoiceChannelSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        IChannelSession::FOnBeginSetTextConnectedCompletedDelegate OnBeginSetTextConnectedCompleteCallback;
        OnBeginSetTextConnectedCompleteCallback.BindLambda(
                [this, &status, vivoxVoiceChannelSession](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Set Text Connected failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                    }
                    completed = true;
                }
        );

        status = fromVivox(vivoxVoiceChannelSession->BeginSetTextConnected(value,
                                                                           OnBeginSetTextConnectedCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return FString::Format(*NSLOCTEXT("Begin Set Text Connected", "channel domain", "%s").ToString(),
                               {VivoxVoiceChannelSession->Channel().Domain()});
    }

#endif
};

void UVivoxChannelSession::BeginSetTextConnected(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        bool value,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginSetTextConnectedAction>(LatentInfo.CallbackTarget,
                                                                                 LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginSetTextConnectedAction(this->VivoxVoiceChannelSession,
                                                                              value,
                                                                              status,
                                                                              LatentInfo));
        }
    }
}

class FBeginSendTextAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    IChannelSession *VivoxVoiceChannelSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginSendTextAction(
            IChannelSession *vivoxVoiceChannelSession,
            const FString &language,
            const FString &message,
            const FString &applicationStanzaNamespace,
            const FString &applicationStanzaBody,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceChannelSession(vivoxVoiceChannelSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        IChannelSession::FOnBeginSendTextCompletedDelegate OnBeginSendTextCompleteCallback;
        OnBeginSendTextCompleteCallback.BindLambda(
                [this, &status, vivoxVoiceChannelSession](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Set Text Connected failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                    }
                    completed = true;
                }
        );

        status = fromVivox(vivoxVoiceChannelSession->BeginSendText(language, message, applicationStanzaNamespace,
                                                                   applicationStanzaBody,
                                                                   OnBeginSendTextCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return FString::Format(*NSLOCTEXT("Begin Set Text Connected", "channel domain", "%s").ToString(),
                               {VivoxVoiceChannelSession->Channel().Domain()});
    }

#endif
};

void UVivoxChannelSession::BeginSendText(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FString &language,
        const FString &message,
        const FString &applicationStanzaNamespace,
        const FString &applicationStanzaBody,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginSendTextAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginSendTextAction(this->VivoxVoiceChannelSession,
                                                                      language,
                                                                      message,
                                                                      applicationStanzaNamespace,
                                                                      applicationStanzaBody,
                                                                      status,
                                                                      LatentInfo));
        }
    }
}

EVivoxStatus UVivoxChannelSession::Set3DPosition(const FVector &speakerPosition,
                                                 const FVector &listenerPosition,
                                                 const FVector &listenerForwardVector,
                                                 const FVector &listenerUpVector) {
    return fromVivox(
            this->VivoxVoiceChannelSession->Set3DPosition(speakerPosition, listenerPosition, listenerForwardVector,
                                                          listenerUpVector));
}

FString UVivoxChannelSession::GetConnectToken(const FString &SecretKey, FTimespan tokenExpirationDuration) {
    return VivoxVoiceChannelSession->GetConnectToken(SecretKey, tokenExpirationDuration);
}

bool UVivoxChannelSession::IsSessionBeingTranscribed() const {
	return this->VivoxVoiceChannelSession->IsSessionBeingTranscribed();
}

class FBeginSetChannelTranscriptionAction : public FPendingLatentAction {
private:
	// trigger for finish
	bool completed = false;
	IChannelSession* VivoxVoiceChannelSession;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

public:
	FBeginSetChannelTranscriptionAction(
		IChannelSession* vivoxVoiceChannelSession,
		bool value,
		const FString& accessToken,
		EVivoxStatus& status,
		const FLatentActionInfo& LatentInfo
	)
		: VivoxVoiceChannelSession(vivoxVoiceChannelSession),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

		IChannelSession::FOnBeginSetChannelTranscriptionCompletedDelegate OnBeginSetChannelTranscriptionCompleteCallback;
		OnBeginSetChannelTranscriptionCompleteCallback.BindLambda(
			[this, &status, vivoxVoiceChannelSession](VivoxCoreError Status) {
				status = fromVivox(Status);
				if (status != EVivoxStatus::Success) {
					LOG_VIVOX_WARNING(FString::Printf(TEXT("Set Text Connected failed: %s"),
						ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
				}
				completed = true;
			}
		);

		status = fromVivox(vivoxVoiceChannelSession->BeginSetChannelTranscription(value, accessToken,
			OnBeginSetChannelTranscriptionCompleteCallback));
	}

	void UpdateOperation(FLatentResponse& Response) override {
		Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
	}

#if WITH_EDITOR

	// Returns a human readable description of the latent operation's current state
	FString GetDescription() const override {
		return FString::Format(*NSLOCTEXT("Begin Set Channel Transcription", "channel domain", "%s").ToString(),
			{ VivoxVoiceChannelSession->Channel().Domain() });
	}

#endif
};

void UVivoxChannelSession::BeginSetChannelTranscription(
	UObject* WorldContextObject,
	struct FLatentActionInfo LatentInfo,
	bool value,
	const FString& accessToken,
	EVivoxStatus& status
) {
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FBeginSetChannelTranscriptionAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
			NULL) {
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
				new FBeginSetChannelTranscriptionAction(this->VivoxVoiceChannelSession,
					value,
					accessToken,
					status,
					LatentInfo));
		}
	}
}

FString UVivoxChannelSession::GetTranscriptionToken(const FString& SecretKey, FTimespan tokenExpirationDuration) {
	return VivoxVoiceChannelSession->GetTranscriptionToken(SecretKey, tokenExpirationDuration);
}