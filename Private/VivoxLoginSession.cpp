/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxLoginSession.h"

#include "ILoginSession.h"
#include "VivoxCore.h"

#include "VivoxChannelSession.h"

#include "LatentActions.h"

#include "VivoxGlobals.h"

UVivoxLoginSession *UVivoxLoginSession::CreateVivoxLoginSession(ILoginSession *VoiceLoginSession) {
    UVivoxLoginSession *vivoxLoginSession = NewObject<UVivoxLoginSession>();
    vivoxLoginSession->VivoxVoiceLoginSession = VoiceLoginSession;

    {
        vivoxLoginSession->SubscriptionRequestReceivedHandle = vivoxLoginSession->VivoxVoiceLoginSession->EventSubscriptionRequestReceived.AddLambda([vivoxLoginSession](const AccountId & vivoxAccountId) {
            if (vivoxLoginSession->SubscriptionRequestReceived.IsBound()) {
                vivoxLoginSession->SubscriptionRequestReceived.Broadcast(FVivoxAccountId().fromVivox(vivoxAccountId));
            }
        });

        vivoxLoginSession->DirectedTextMessageReceivedHandle = vivoxLoginSession->VivoxVoiceLoginSession->EventDirectedTextMessageReceived.AddLambda([vivoxLoginSession](const IDirectedTextMessage & vivoxDirectedTextMessage) {
            if (vivoxLoginSession->DirectedTextMessageReceived.IsBound()) {
                vivoxLoginSession->DirectedTextMessageReceived.Broadcast(UVivoxDirectedTextMessage::CreateVivoxDirectedTextMessage((IDirectedTextMessage *)&vivoxDirectedTextMessage));
            }
        });

        vivoxLoginSession->SendDirectedTextMessageFailedHandle = vivoxLoginSession->VivoxVoiceLoginSession->EventSendDirectedTextMessageFailed.AddLambda([vivoxLoginSession](VivoxCoreError vivoxStatus, const FString &delegateMessage) {
            if (vivoxLoginSession->SendDirectedTextMessageFailed.IsBound()) {
                vivoxLoginSession->SendDirectedTextMessageFailed.Broadcast(fromVivox(vivoxStatus), delegateMessage);
            }
        });

        vivoxLoginSession->StateChangedHandle = vivoxLoginSession->VivoxVoiceLoginSession->EventStateChanged.AddLambda([vivoxLoginSession](LoginState vivoxLoginState) {
            if (vivoxLoginSession->StateChanged.IsBound()) {
                switch (vivoxLoginState) {
                    case LoginState::LoggedIn:
                        vivoxLoginSession->StateChanged.Broadcast(EVivoxLoginState::LoggedIn);
                        return;
                    case LoginState::LoggedOut:
                        vivoxLoginSession->StateChanged.Broadcast(EVivoxLoginState::LoggedOut);
                        return;
                    case LoginState::LoggingIn:
                        vivoxLoginSession->StateChanged.Broadcast(EVivoxLoginState::LoggingIn);
                        return;
                    case LoginState::LoggingOut:
                        vivoxLoginSession->StateChanged.Broadcast(EVivoxLoginState::LoggingOut);
                        return;
                }
            }
        });

        vivoxLoginSession->AudioInjectionCompletedHandle = vivoxLoginSession->VivoxVoiceLoginSession->EventAudioInjectionCompleted.AddLambda([vivoxLoginSession]() {
            if (vivoxLoginSession->AudioInjectionCompleted.IsBound()) {
                vivoxLoginSession->AudioInjectionCompleted.Broadcast();
            }
        });
    }

    return vivoxLoginSession;
}

void UVivoxLoginSession::BeginDestroy() {
	this->SubscriptionRequestReceived.Clear();
	this->DirectedTextMessageReceived.Clear();
	this->SendDirectedTextMessageFailed.Clear();
	this->StateChanged.Clear();
	this->AudioInjectionCompleted.Clear();

	if (this->VivoxVoiceLoginSession != nullptr) {
		this->VivoxVoiceLoginSession->EventSubscriptionRequestReceived.Remove(this->SubscriptionRequestReceivedHandle);
		this->VivoxVoiceLoginSession->EventDirectedTextMessageReceived.Remove(this->DirectedTextMessageReceivedHandle);
		this->VivoxVoiceLoginSession->EventSendDirectedTextMessageFailed.Remove(this->SendDirectedTextMessageFailedHandle);
		this->VivoxVoiceLoginSession->EventStateChanged.Remove(this->StateChangedHandle);
		this->VivoxVoiceLoginSession->EventAudioInjectionCompleted.Remove(this->AudioInjectionCompletedHandle);
	}

	this->VivoxVoiceLoginSession = nullptr;
	this->UObject::BeginDestroy();
}

TMap<FVivoxChannelId, UVivoxChannelSession *> UVivoxLoginSession::ChannelSessions() const {
    TMap<FVivoxChannelId, UVivoxChannelSession * > channelSessions;

    for (const TPair<ChannelId, TSharedPtr<IChannelSession> > &vivoxChannelSession : this->VivoxVoiceLoginSession->ChannelSessions()) {
        channelSessions.Add(FVivoxChannelId().fromVivox(vivoxChannelSession.Key),
                            UVivoxChannelSession::CreateVivoxChannelSession(vivoxChannelSession.Value.Get()));
    }

    return channelSessions;
}

TMap<FVivoxAccountId, UVivoxPresenceSubscription *> UVivoxLoginSession::PresenceSubscriptions() const {
    TMap<FVivoxAccountId, UVivoxPresenceSubscription * > presenceSubscriptions;
    for (const TPair<AccountId, TSharedPtr<IPresenceSubscription> > &vivoxPresenceSubscriptions : this->VivoxVoiceLoginSession->PresenceSubscriptions()) {
        presenceSubscriptions.Add(FVivoxAccountId().fromVivox(vivoxPresenceSubscriptions.Key),
                                  UVivoxPresenceSubscription::CreateVivoxPresenceSubscription(
                                          vivoxPresenceSubscriptions.Value.Get()));
    }

    return presenceSubscriptions;
}

TSet<FVivoxAccountId> UVivoxLoginSession::BlockedSubscriptions() const {
    TSet<FVivoxAccountId> blockedSubscriptions;

    for (const AccountId &vivoxBlockedSubscription : this->VivoxVoiceLoginSession->BlockedSubscriptions()) {
        blockedSubscriptions.Add(FVivoxAccountId().fromVivox(vivoxBlockedSubscription));
    }

    return blockedSubscriptions;
}

TSet<FVivoxAccountId> UVivoxLoginSession::AllowedSubscriptions() const {
    TSet<FVivoxAccountId> allowedSubscriptions;

    for (const AccountId &vivoxAllowedSubscription : this->VivoxVoiceLoginSession->AllowedSubscriptions()) {
        allowedSubscriptions.Add(FVivoxAccountId().fromVivox(vivoxAllowedSubscription));
    }

    return allowedSubscriptions;
}

EVivoxLoginState UVivoxLoginSession::State() const {
    switch (this->VivoxVoiceLoginSession->State()) {
        case LoginState::LoggedOut:
            return EVivoxLoginState::LoggedOut;
            break;
        case LoginState::LoggingIn:
            return EVivoxLoginState::LoggingIn;
            break;
        case LoginState::LoggedIn:
            return EVivoxLoginState::LoggedIn;
            break;
        case LoginState::LoggingOut:
            return EVivoxLoginState::LoggingOut;
            break;
        default:
			return EVivoxLoginState::LoggedOut;
            break;
    }
}

UVivoxPresence *UVivoxLoginSession::CurrentPresence() {
    return UVivoxPresence::CreateVivoxPresence((Presence *)&(this->VivoxVoiceLoginSession->CurrentPresence()));
}

EVivoxStatus UVivoxLoginSession::SetCurrentPresence(const UVivoxPresence * value) {
    return fromVivox(this->VivoxVoiceLoginSession->SetCurrentPresence(*(value->VivoxPresence)));
}

FVivoxAccountId UVivoxLoginSession::LoginSessionId() const {
    return FVivoxAccountId().fromVivox(this->VivoxVoiceLoginSession->LoginSessionId());
}

class FBeginLoginAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginLoginAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FString &server,
            const FString &accessToken,
            EVivoxSubscriptionMode subscriptionMode,
            const TSet<FVivoxAccountId> &presenceSubscriptions,
            const TSet<FVivoxAccountId> &blockedPresenceSubscriptions,
            const TSet<FVivoxAccountId> &allowedPresenceSubscriptions,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginLoginCompletedDelegate OnBeginLoginCompleteCallback;
        OnBeginLoginCompleteCallback.BindLambda([this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
            status = fromVivox(Status);
            if (status != EVivoxStatus::Success) {
                LOG_VIVOX_WARNING(FString::Printf(TEXT("Login failed: %s"),
                                                  ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                completed = true;
            } else {
                completed = true;

            }
        });


        SubscriptionMode vivoxSubscriptionMode = SubscriptionMode::Defer;

        switch (subscriptionMode) {
            case EVivoxSubscriptionMode::Accept:
                vivoxSubscriptionMode = SubscriptionMode::Accept;
                break;
            case EVivoxSubscriptionMode::Block:
                vivoxSubscriptionMode = SubscriptionMode::Block;
                break;
            case EVivoxSubscriptionMode::Defer:
                vivoxSubscriptionMode = SubscriptionMode::Defer;
                break;
            default:
                break;
        }

        TSet<AccountId> vivoxPresenceSubscriptions;

        for (const FVivoxAccountId &presenceSubscription : presenceSubscriptions) {
            vivoxPresenceSubscriptions.Add(presenceSubscription.toVivox());
        }

        TSet<AccountId> vivoxBlockedPresenceSubscriptions;

        for (const FVivoxAccountId &blockedPresenceSubscription : blockedPresenceSubscriptions) {
            vivoxBlockedPresenceSubscriptions.Add(blockedPresenceSubscription.toVivox());
        }

        TSet<AccountId> vivoxAllowedPresenceSubscriptions;

        for (const FVivoxAccountId &allowedPresenceSubscription : allowedPresenceSubscriptions) {
            vivoxAllowedPresenceSubscriptions.Add(allowedPresenceSubscription.toVivox());
        }

        status = fromVivox(vivoxVoiceLoginSession->BeginLogin(server, accessToken, vivoxSubscriptionMode,
                                                              vivoxPresenceSubscriptions,
                                                              vivoxBlockedPresenceSubscriptions,
                                                              vivoxAllowedPresenceSubscriptions,
                                                              OnBeginLoginCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Login Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginLogin(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FString &server,
        const FString &accessToken,
        EVivoxSubscriptionMode subscriptionMode,
        const TSet<FVivoxAccountId> &presenceSubscriptions,
        const TSet<FVivoxAccountId> &blockedPresenceSubscriptions,
        const TSet<FVivoxAccountId> &allowedPresenceSubscriptions,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginLoginAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginLoginAction(this->VivoxVoiceLoginSession,
                                                                   server,
                                                                   accessToken,
                                                                   subscriptionMode,
                                                                   presenceSubscriptions,
                                                                   blockedPresenceSubscriptions,
                                                                   allowedPresenceSubscriptions,
                                                                   status,
                                                                   LatentInfo));
        }
    }
}

UVivoxChannelSession *UVivoxLoginSession::GetChannelSession(const FVivoxChannelId &channelId) {
    return UVivoxChannelSession::CreateVivoxChannelSession(
            &(VivoxVoiceLoginSession->GetChannelSession(channelId.toVivox())));
}

void UVivoxLoginSession::DeleteChannelSession(const FVivoxChannelId &channelId) {
    VivoxVoiceLoginSession->DeleteChannelSession(channelId.toVivox());
}

class FBeginAddBlockedSubscriptionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginAddBlockedSubscriptionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginAddBlockedSubscriptionCompletedDelegate OnBeginAddBlockedSubscriptionCompleteCallback;
        OnBeginAddBlockedSubscriptionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Add Blocked Subscription failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(vivoxVoiceLoginSession->BeginAddBlockedSubscription(accountId.toVivox(),
                                                                               OnBeginAddBlockedSubscriptionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Add Blocked Subscription Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginAddBlockedSubscription(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginAddBlockedSubscriptionAction>(LatentInfo.CallbackTarget,
                                                                                       LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginAddBlockedSubscriptionAction(this->VivoxVoiceLoginSession,
                                                                                    accountId,
                                                                                    status,
                                                                                    LatentInfo));
        }
    }
}

class FBeginRemoveBlockedSubscriptionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginRemoveBlockedSubscriptionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginRemoveBlockedSubscriptionCompletedDelegate OnBeginRemoveBlockedSubscriptionCompleteCallback;
        OnBeginRemoveBlockedSubscriptionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Remove Blocked Subscription failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(vivoxVoiceLoginSession->BeginRemoveBlockedSubscription(accountId.toVivox(),
                                                                                  OnBeginRemoveBlockedSubscriptionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Remove Blocked Subscription Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginRemoveBlockedSubscription(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginRemoveBlockedSubscriptionAction>(LatentInfo.CallbackTarget,
                                                                                          LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginRemoveBlockedSubscriptionAction(this->VivoxVoiceLoginSession,
                                                                                       accountId,
                                                                                       status,
                                                                                       LatentInfo));
        }
    }
}

class FBeginAddAllowedSubscriptionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginAddAllowedSubscriptionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginAddAllowedSubscriptionCompletedDelegate OnBeginAddAllowedSubscriptionCompleteCallback;
        OnBeginAddAllowedSubscriptionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Add Allowed Subscription failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(
                vivoxVoiceLoginSession->BeginAddAllowedSubscription(accountId.toVivox(), OnBeginAddAllowedSubscriptionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Add Allowed Subscription Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginAddAllowedSubscription(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginAddAllowedSubscriptionAction>(LatentInfo.CallbackTarget,
                                                                                       LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginAddAllowedSubscriptionAction(this->VivoxVoiceLoginSession,
                                                                                    accountId,
                                                                                    status,
                                                                                    LatentInfo));
        }
    }
}

class FBeginRemoveAllowedSubscriptionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginRemoveAllowedSubscriptionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginRemoveAllowedSubscriptionCompletedDelegate OnBeginRemoveAllowedSubscriptionCompleteCallback;
        OnBeginRemoveAllowedSubscriptionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Remove Allowed Subscription failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(vivoxVoiceLoginSession->BeginRemoveAllowedSubscription(accountId.toVivox(),
                                                                                  OnBeginRemoveAllowedSubscriptionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Remove Allowed Subscription Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginRemoveAllowedSubscription(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginRemoveAllowedSubscriptionAction>(LatentInfo.CallbackTarget,
                                                                                          LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginRemoveAllowedSubscriptionAction(this->VivoxVoiceLoginSession,
                                                                                       accountId,
                                                                                       status,
                                                                                       LatentInfo));
        }
    }
}

class FBeginAddPresenceSubscriptionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginAddPresenceSubscriptionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginAddPresenceSubscriptionCompletedDelegate OnBeginAddPresenceSubscriptionCompleteCallback;
        OnBeginAddPresenceSubscriptionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Add Presence Subscription failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(vivoxVoiceLoginSession->BeginAddPresenceSubscription(accountId.toVivox(),
                                                                                OnBeginAddPresenceSubscriptionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Add Presence Subscription Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginAddPresenceSubscription(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginAddPresenceSubscriptionAction>(LatentInfo.CallbackTarget,
                                                                                        LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginAddPresenceSubscriptionAction(this->VivoxVoiceLoginSession,
                                                                                     accountId,
                                                                                     status,
                                                                                     LatentInfo));
        }
    }
}

class FBeginRemovePresenceSubscriptionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginRemovePresenceSubscriptionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginRemovePresenceSubscriptionCompletedDelegate OnBeginRemovePresenceSubscriptionCompleteCallback;
        OnBeginRemovePresenceSubscriptionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Remove Presence Subscription failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(vivoxVoiceLoginSession->BeginRemovePresenceSubscription(accountId.toVivox(),
                                                                                   OnBeginRemovePresenceSubscriptionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Remove Presence Subscription Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginRemovePresenceSubscription(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginRemovePresenceSubscriptionAction>(LatentInfo.CallbackTarget,
                                                                                           LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginRemovePresenceSubscriptionAction(this->VivoxVoiceLoginSession,
                                                                                        accountId,
                                                                                        status,
                                                                                        LatentInfo));
        }
    }
}

class FBeginSendSubscriptionReplyAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginSendSubscriptionReplyAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            const EVivoxSubscriptionReply &replyType,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginSendSubscriptionReplyCompletedDelegate OnBeginSendSubscriptionReplyCompleteCallback;
        OnBeginSendSubscriptionReplyCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Send Subscription Reply failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        SubscriptionReply vivoxSubscriptionReply;

        switch (replyType) {
            case EVivoxSubscriptionReply::Allow:
                vivoxSubscriptionReply = SubscriptionReply::Allow;
                break;
            case EVivoxSubscriptionReply::Block:
                vivoxSubscriptionReply = SubscriptionReply::Block;
                break;
            default:
                vivoxSubscriptionReply = SubscriptionReply::Block;
                break;
        }

        status = fromVivox(
                vivoxVoiceLoginSession->BeginSendSubscriptionReply(accountId.toVivox(), vivoxSubscriptionReply,
                                                                   OnBeginSendSubscriptionReplyCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Send Subscription Reply Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginSendSubscriptionReply(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        const EVivoxSubscriptionReply &replyType,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginSendSubscriptionReplyAction>(LatentInfo.CallbackTarget,
                                                                                      LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginSendSubscriptionReplyAction(this->VivoxVoiceLoginSession,
                                                                                   accountId,
                                                                                   replyType,
                                                                                   status,
                                                                                   LatentInfo));
        }
    }
}

class FBeginSendDirectedMessageAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginSendDirectedMessageAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FVivoxAccountId &accountId,
            const FString &language,
            const FString &message,
            const FString &applicationStanzaNamespace,
            const FString &applicationStanzaBody,
            EVivoxStatus &status,
            FString &delegateMessage,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginSendDirectedMessageCompletedDelegate OnBeginSendDirectedMessageCompleteCallback;
        OnBeginSendDirectedMessageCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status, &delegateMessage](VivoxCoreError Status, const FString & DelegateMessage) {
                    status = fromVivox(Status);
                    delegateMessage = DelegateMessage;
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Send Directed Message failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(vivoxVoiceLoginSession->BeginSendDirectedMessage(accountId.toVivox(), language, message,
                                                                            applicationStanzaNamespace,
                                                                            applicationStanzaBody,
                                                                            OnBeginSendDirectedMessageCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Send Directed Message Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginSendDirectedMessage(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FVivoxAccountId &accountId,
        const FString &language,
        const FString &message,
        const FString &applicationStanzaNamespace,
        const FString &applicationStanzaBody,
        EVivoxStatus &status,
        FString &delegateMessage
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginSendDirectedMessageAction>(LatentInfo.CallbackTarget,
                                                                                    LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginSendDirectedMessageAction(this->VivoxVoiceLoginSession,
                                                                                 accountId,
                                                                                 language,
                                                                                 message,
                                                                                 applicationStanzaNamespace,
                                                                                 applicationStanzaBody,
                                                                                 status,
                                                                                 delegateMessage,
                                                                                 LatentInfo));
        }
    }
}

class FBeginStartAudioInjectionAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    ILoginSession *VivoxVoiceLoginSession;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginStartAudioInjectionAction(
            ILoginSession *vivoxVoiceLoginSession,
            const FString &filePath,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxVoiceLoginSession(vivoxVoiceLoginSession),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        ILoginSession::FOnBeginStartAudioInjectionCompletedDelegate OnBeginStartAudioInjectionCompleteCallback;
        OnBeginStartAudioInjectionCompleteCallback.BindLambda(
                [this, &vivoxVoiceLoginSession, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Start Audio Injection failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(
                vivoxVoiceLoginSession->BeginStartAudioInjection(filePath, OnBeginStartAudioInjectionCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Start Audio Injection Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxLoginSession::BeginStartAudioInjection(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        const FString &filePath,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginStartAudioInjectionAction>(LatentInfo.CallbackTarget,
                                                                                    LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginStartAudioInjectionAction(this->VivoxVoiceLoginSession,
                                                                                 filePath,
                                                                                 status,
                                                                                 LatentInfo));
        }
    }
}

EVivoxStatus UVivoxLoginSession::StopAudioInjection() {
    return fromVivox(this->VivoxVoiceLoginSession->StopAudioInjection());
}

bool UVivoxLoginSession::IsAudioInjecting() {
    return this->VivoxVoiceLoginSession->IsAudioInjecting();
}

EVivoxStatus UVivoxLoginSession::SetTransmissionMode(EVivoxTransmissionMode mode, FVivoxChannelId singleChannel) {
	TransmissionMode vivoxMode = TransmissionMode::None;

	switch (mode) {
	case EVivoxTransmissionMode::None:
		vivoxMode = TransmissionMode::None;
		break;
	case EVivoxTransmissionMode::Single:
		vivoxMode = TransmissionMode::Single;
		break;
	case EVivoxTransmissionMode::All:
		vivoxMode = TransmissionMode::All;
		break;
	default:
		vivoxMode = TransmissionMode::None;
		break;
	}

	return fromVivox(this->VivoxVoiceLoginSession->SetTransmissionMode(vivoxMode, singleChannel.toVivox()));
}

EVivoxTransmissionMode UVivoxLoginSession::GetTransmissionMode() const {
	switch (this->VivoxVoiceLoginSession->GetTransmissionMode())
	{
	case TransmissionMode::None:
		return EVivoxTransmissionMode::None;
		break;
	case TransmissionMode::Single:
		return EVivoxTransmissionMode::Single;
		break;
	case TransmissionMode::All:
		return EVivoxTransmissionMode::All;
		break;
	default:
		return EVivoxTransmissionMode::None;
		break;
	}
}

TArray<FVivoxChannelId> UVivoxLoginSession::GetTransmittingChannels() const {
	TArray<ChannelId> vivoxTransmittingChannels = this->VivoxVoiceLoginSession->GetTransmittingChannels();
	TArray<FVivoxChannelId> transmittingChannels;

	for (const ChannelId& vivoxChannelId : vivoxTransmittingChannels) {
		transmittingChannels.Add(FVivoxChannelId().fromVivox(vivoxChannelId));
	}

	return transmittingChannels;
}

EVivoxStatus UVivoxLoginSession::SetParticipantSpeakingUpdateRate(EVivoxParticipantSpeakingUpdateRate rate) {
	ParticipantSpeakingUpdateRate vivoxRate = ParticipantSpeakingUpdateRate::Never;

	switch (rate) {
	case EVivoxParticipantSpeakingUpdateRate::Never:
		vivoxRate = ParticipantSpeakingUpdateRate::Never;
		break;
	case EVivoxParticipantSpeakingUpdateRate::StateChange:
		vivoxRate = ParticipantSpeakingUpdateRate::StateChange;
		break;
	case EVivoxParticipantSpeakingUpdateRate::Update1Hz:
		vivoxRate = ParticipantSpeakingUpdateRate::Update1Hz;
		break;
	case EVivoxParticipantSpeakingUpdateRate::Update5Hz:
		vivoxRate = ParticipantSpeakingUpdateRate::Update5Hz;
		break;
	case EVivoxParticipantSpeakingUpdateRate::Update10Hz:
		vivoxRate = ParticipantSpeakingUpdateRate::Update10Hz;
		break;
	default:
		vivoxRate = ParticipantSpeakingUpdateRate::Never;
		break;
	}

	return fromVivox(this->VivoxVoiceLoginSession->SetParticipantSpeakingUpdateRate(vivoxRate));

}

EVivoxParticipantSpeakingUpdateRate UVivoxLoginSession::GetParticipantSpeakingUpdateRate() const {
	switch (this->VivoxVoiceLoginSession->GetParticipantSpeakingUpdateRate())
	{
	case ParticipantSpeakingUpdateRate::Never:
		return EVivoxParticipantSpeakingUpdateRate::Never;
		break;
	case ParticipantSpeakingUpdateRate::StateChange:
		return EVivoxParticipantSpeakingUpdateRate::StateChange;
		break;
	case ParticipantSpeakingUpdateRate::Update1Hz:
		return EVivoxParticipantSpeakingUpdateRate::Update1Hz;
		break;
	case ParticipantSpeakingUpdateRate::Update5Hz:
		return EVivoxParticipantSpeakingUpdateRate::Update5Hz;
		break;
	case ParticipantSpeakingUpdateRate::Update10Hz:
		return EVivoxParticipantSpeakingUpdateRate::Update10Hz;
		break;
	default:
		return EVivoxParticipantSpeakingUpdateRate::Never;
		break;
	}
}

void UVivoxLoginSession::Logout() {
    this->VivoxVoiceLoginSession->Logout();
}

FString UVivoxLoginSession::GetLoginToken(const FString &SecretKey, FTimespan tokenExpirationDuration) {
    return VivoxVoiceLoginSession->GetLoginToken(SecretKey, tokenExpirationDuration);
}