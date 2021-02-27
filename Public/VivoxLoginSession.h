/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"
#include "Delegates/IDelegateInstance.h"

#include "VivoxCore.h"

#include "VivoxStatus.h"
#include "VivoxAccountId.h"
#include "VivoxChannelId.h"
#include "VivoxPresence.h"
#include "VivoxPresenceSubscription.h"

#include "VivoxLoginSession.generated.h"

class UVivoxChannelSession;
class UVivoxDirectedTextMessage;

/**
 * \brief The state of the login session
 */
UENUM(BlueprintType)
enum class EVivoxLoginState : uint8
{
    /**
     * \brief Login Session is logged out
     */
            LoggedOut	UMETA(DisplayName = "logged out"),
    /**
     * \brief Login Session is in the process of logging in
     */
            LoggingIn	UMETA(DisplayName = "logging in"),
    /**
     * \brief Login Session is logged in
     */
            LoggedIn	UMETA(DisplayName = "logged in"),
    /**
     * \brief Login Session is in the process of logging out.
     */
            LoggingOut	UMETA(DisplayName = "logging out"),
};

/**
 * \brief How to handle incoming subscriptions
 */
UENUM(BlueprintType)
enum class EVivoxSubscriptionMode : uint8
{
    /**
     * \brief Automatically accept all incoming subscription requests.
     */
            Accept			UMETA(DisplayName = "accept"),
			/**
			 * \brief Automatically block all incoming subscription requests.
			 */
			 Block UMETA(DisplayName = "block"),
    /**
     * \brief Defer incoming subscription request handling to the application.
     * This will result in EventSubscriptionRequestReceived firing
     */
            Defer			UMETA(DisplayName = "defer")
};

UENUM(BlueprintType)
enum class EVivoxSubscriptionReply : uint8
{
    /**
    * \brief Allow the incoming subscription
    */
            Allow			UMETA(DisplayName = "allow"),
    /**
    * \brief Block the incoming subscription
    */
            Block			UMETA(DisplayName = "block")
};

/**
 * \brief Defines the policy of where microphone audio and injected audio get broadcast.
 */
UENUM(BlueprintType)
enum class EVivoxTransmissionMode : uint8
{
	/**
	 * \brief Adopts a policy of transmission into no channels.
	 */
	None UMETA(DisplayName = "none"),
	/**
	 * \brief Adopts a policy of transmission into one channel at a time.
	 */
	 Single UMETA(DisplayName = "single"),
	 /**
	  * \brief Adopts a policy of transmission into all channels at once.
	  */
	  All UMETA(DisplayName = "all")
};

/**
 * \brief Defines how often the SDK raises <b>speech related</b> IChannelSession::EventAfterParticipantUpdated events while in channel.
 * \remarks Use a per second rate (at the fidelity of your choice) when implementing a real-time audio energy meter.
 * This controls participant update events due to changes in speech detection or changes in audio energy, only.
 * Other participant property state change events (active media, muted state, etc.) will occur regardless of setting.
 */
UENUM(BlueprintType)
enum class EVivoxParticipantSpeakingUpdateRate : uint8
{
	/**
	 * \brief On participant state change in speech detection only (excludes audio energy).
	 */
	StateChange UMETA(DisplayName = "state change"),
	/**
	 * \brief Don't raise events for changes in participant speech detection or audio energy (use with caution).
	 */
	 Never UMETA(DisplayName = "never"),
	 /**
	  * \brief Update participant properties up to 1 time per second (includes audio energy).
	  */
	  Update1Hz UMETA(DisplayName = "update 1 hz"),
	  /**
	   * \brief Update participant properties up to 5 times per second (includes audio energy).
	   */
	   Update5Hz UMETA(DisplayName = "update 5 hz"),
	   /**
		* \brief Update participant properties up to 10 times per second (includes audio energy).
		*/
		Update10Hz UMETA(DisplayName = "update 10 hz")
};

UCLASS(BlueprintType)
class VIVOX_API UVivoxLoginSession : public UObject {
public:
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSubscriptionRequestReceived, const FVivoxAccountId &, vivoxAccountId);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDirectedTextMessageReceived, const UVivoxDirectedTextMessage *, vivoxDirectedTextMessage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendDirectedTextMessageFailed, EVivoxStatus, vivoxStatus, const FString &, delegateMessage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChanged, EVivoxLoginState, vivoxLoginState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioInjectionCompleted);

public:
    static UVivoxLoginSession* CreateVivoxLoginSession(ILoginSession *VivoxVoiceLoginSession);

    /**
    * \brief An event indicating that a new subscription request has been received
    * \remarks That is, another player would like to be your buddy.
    */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FSubscriptionRequestReceived SubscriptionRequestReceived;

    /**
     * \brief An event indicating that another player has sent you a text message
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FDirectedTextMessageReceived DirectedTextMessageReceived;

    /**
     * \brief An event indicating a directed message delivery failed.
     *
     * \remarks The VivoxCoreError parameter specifies the reason for failure and the FString parameter
     *  is a request ID matching the one returned in FOnBeginSendDirectedMessageCompletedDelegate when
     *  the message was sent. Match this request ID with the one returned in the delegate to determine
     *  which directed message failed to be delivered.
     */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FSendDirectedTextMessageFailed SendDirectedTextMessageFailed;

    /**
    * \brief The event fired when State() changes, except on player initiated logouts.
    * \remarks Loss of network connectivity will cause this event to fire
    */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FStateChanged StateChanged;

    /**
    * \brief An event indicating that you are no longer injecting audio
    * \remarks This event is raised either if injection is stopped early with StopAudioInjection()
    *  or the file you're injecting audio from has reached its natural end.
    */
    UPROPERTY(BlueprintAssignable, Category="vivox")
    FAudioInjectionCompleted AudioInjectionCompleted;

    FDelegateHandle SubscriptionRequestReceivedHandle;
    FDelegateHandle DirectedTextMessageReceivedHandle;
    FDelegateHandle SendDirectedTextMessageFailedHandle;
    FDelegateHandle StateChangedHandle;
    FDelegateHandle AudioInjectionCompletedHandle;

	virtual void BeginDestroy() override;

    /**
     * \brief The list of channel sessions associated with this login session
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    TMap<FVivoxChannelId, UVivoxChannelSession *> ChannelSessions() const;

    /**
    * \brief The list of presence subscriptions associated with this login session
    * \remarks This typically corresponds to a list of "friends" or "followers"
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    TMap <FVivoxAccountId, UVivoxPresenceSubscription * > PresenceSubscriptions() const;

    /**
     * \brief The list of accounts blocked from seeing this accounts online status
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    TSet<FVivoxAccountId> BlockedSubscriptions() const;

    /**
    * \brief The list of accounts allowed to see this accounts online status
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    TSet<FVivoxAccountId> AllowedSubscriptions() const;

    /**
     * \brief The current state of this login session
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    EVivoxLoginState State() const;

    /**
    * \brief The online status that is sent to those accounts subscribing to the presence of this account
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    UVivoxPresence *CurrentPresence();

    /**
    * \brief Set this player's online status
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    EVivoxStatus SetCurrentPresence(const UVivoxPresence * value);

    /**
    * \brief The unique identifier for this LoginSession
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    FVivoxAccountId LoginSessionId() const;

    /**
    * \brief Begin logging in this session, with presence
    *
    * \param server The URI of the Vivox instance assigned to you
    * \param accessToken an access token provided by your game server that enables this login
    * \param subscriptionMode how to handle incoming subscriptions
    * \param presenceSubscriptions A list of accounts for which this user wishes to monitor online status
    * \param blockedPresenceSubscriptions A list of accounts that are not allowed to see this user's online status
    * \param allowedPresenceSubscriptions A list of accounts that are allowed to see this user's online status
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success
    * \remarks
    * Developer of games that do not have secure communications requirements can use GetLoginToken to generate the required access token.
    */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginLogin(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FString &server,
            const FString &accessToken,
            EVivoxSubscriptionMode subscriptionMode,
            const TSet<FVivoxAccountId> & presenceSubscriptions,
            const TSet<FVivoxAccountId> & blockedPresenceSubscriptions,
            const TSet<FVivoxAccountId> & allowedPresenceSubscriptions,
            EVivoxStatus &status
    );

    /**
    * \brief Gets the channel session for this channelId, creating one if necessary
    *
    * \param channelId the id of the channel
    * \return the channel session
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    UVivoxChannelSession* GetChannelSession(const FVivoxChannelId &channelId);

    /**
    * \brief Deletes the channel session for this channelId, disconnecting the session if necessary
    *
    * \param channelId the id of the channel
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    void DeleteChannelSession(const FVivoxChannelId &channelId);

    /**
     * \brief Block incoming subscription requests from the specified account
     *
     * \param accountId the account id to block
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginAddBlockedSubscription(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status
    );

    /**
     * \brief Unblock incoming subscription requests from the specified account. Subscription requests from the specified account will cause an event to be raised to the application.
     *
     * \param accountId the account id to unblock
     * \param theDelegate a delegate to call when this operation completes
     * @return 0 on success
     */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginRemoveBlockedSubscription(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status
    );

    /**
     * \brief Allow incoming subscription requests from the specified account
     *
     * \param accountId the account id to allow
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginAddAllowedSubscription(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status
    );

    /**
     * \brief Disallow automatic acceptance of incoming subscription requests from the specified account. Subscription requests from the specified account will cause an event to be raised to the application.
     *
     * \param accountId the account id to disallow
     * \param theDelegate a delegate to call when this operation completes
     * \return 0 on success
     */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginRemoveAllowedSubscription(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status
    );

    /**
    * \brief Subscribe to the specified account
    *
    * \param accountId the account id to subscribe to
    * \param theDelegate a delegate to call when this operation completes
    * \remarks This method will automatically allow accountId to see the subscriber's online status
    * \return 0 on success
    */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginAddPresenceSubscription(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status
    );

    /**
    * \brief Unsubscribe from the specified account
    *
    * \param accountId the account id to subscribe to
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success
    */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginRemovePresenceSubscription(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            EVivoxStatus &status
    );

    /**
    * \brief Reply to a subscription request received via EventSubscriptionRequestReceived
    *
    * \param accountId the accountId to reply to
    * \param replyType the SubscriptionReply type to use: Allow or Block
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success
    */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginSendSubscriptionReply(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            const EVivoxSubscriptionReply & replyType,
            EVivoxStatus &status
    );

    /**
    * \brief Send a message to the specific account
    *
    * \param accountId the intended recipient of the message
    * \param language the language of the message e.g "en". This can be null to use the default language ("en" for most systems). This must conform to RFC5646 (https://tools.ietf.org/html/rfc5646)
    * \param message the body of the message
    * \param applicationStanzaNamespace an optional namespace element for additional application data
    * \param applicationStanzaBody the additional application data body
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidOperation if BeginLogin was called with no presence enabled (short form)
    */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginSendDirectedMessage(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FVivoxAccountId &accountId,
            const FString &language,
            const FString & message,
            const FString & applicationStanzaNamespace,
            const FString & applicationStanzaBody,
            EVivoxStatus &status,
            FString &delegateMessage
    );

    /**
    * \brief Start broadcasting audio from a file to all connected ChannelSessions (see remarks)
    *
    * \param filePath the full pathname for the WAV file to play (MUST be single channel, 16-bit PCM)
    * \param theDelegate a delegate to call when this operation completes
    * \return 0 on success, VxErrorInvalidState if not in Audio Connected ChannelSession
    *
    * \remarks Injected audio is treated like a second microphone you're speaking into. This
    *  means that muting or disconnecting your input device won't stop others from hearing
    *  the file audio, but muting yourself in a channel, stopping transmission to a channel,
    *  or disconnecting channel AudioState will. See Developer Guide for more details.
    */
    UFUNCTION(BlueprintCallable,
            meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
            Category = "vivox"
    )
    void BeginStartAudioInjection(
            UObject *WorldContextObject,
            struct FLatentActionInfo LatentInfo,
            const FString &filePath,
            EVivoxStatus &status
    );

    /**
    * \brief Stop broadcasting audio from a file
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    EVivoxStatus StopAudioInjection();

    /**
    * \brief True if currently broadcasting audio from a file
    */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    bool IsAudioInjecting();

	/**
 * \brief Set whether microphone audio and injected audio should be transmitted to no channels, all channels, or a single specific channel.
 * \param mode enum specifying a transmission policy
 * \param singleChannel the specific channel to transmit to when TransmissionMode::Single is set (ignored otherwise)
 * \remarks Audio transmission changes take effect immediately and also affect future channel joins as follows:
 * - <b>None:</b> audio will not automatically transmit to new channels nor to text-only channels when audio is added.
 * - <b>All:</b> audio automatically transmits to new channels and to text-only channels when audio is added.
 * - <b>Single:</b> audio will transmit into the channel specified only, and will not automatically switch to new channels even if this channel is disconnected.
 *
 * <b>Important:</b> You can override and change this setting by passing `true` for the switchTransmission argument in IChannelSession::BeginConnect() and IChannelSession::BeginSetAudioConnected().
 * \return VxErrorInvalidArgument if mode is TransmissionMode::Single and default/no ChannelId argument is provided
 */
	UFUNCTION(BlueprintCallable, Category = "vivox")
	EVivoxStatus SetTransmissionMode(EVivoxTransmissionMode mode, FVivoxChannelId singleChannel);

	/**
	 * \brief Get the transmission policy set for the player.
	 * \return the active TransmissionMode
	 * \see SetTransmissionMode()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	EVivoxTransmissionMode GetTransmissionMode() const;

	/**
	 * \brief Get the ChannelId of each channel that will broadcast non-muted microphone input and injected audio.
	 * \return an array of transmitting ChannelIds
	 * \see SetTransmissionMode()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	TArray<FVivoxChannelId> GetTransmittingChannels() const;

	/**
	 * \brief Set how often the SDK will raise IChannelSession::EventAfterParticipantUpdated events relating to speaking activity.
	 * \param rate enum specifying the desired frequency of participant events due to changes in speech detection or audio energy
	 * \remarks By default, the Vivox SDK is set up to support a simple speaking/not-speaking indicator for channel participants;
	 * for this use case, keeping the default value of ParticipantSpeakingUpdateRate::StateChange is sufficient. If you'd like to
	 * create a dynamic voice activity indicator or other real-time visual representation of audio energy levels, supply this
	 * function with a ParticipantSpeakingUpdateRate value of Update1Hz, Update5Hz, or Update10Hz for updates up to 1, 5, or 10
	 * times per second as audio energy and other participant properties change. See the Unreal Developer Guide for more information.
	 *
	 * <b>Important:</b> While required for displaying real-time voice levels, setting a ParticipantSpeakingUpdateRate of any per
	 * second rate will necessarily increase network traffic between the application and Vivox servers.
	 * \return 0 on success
	 */
	UFUNCTION(BlueprintCallable, Category = "vivox")
	EVivoxStatus SetParticipantSpeakingUpdateRate(EVivoxParticipantSpeakingUpdateRate rate);

	/**
	 * \brief Get the frequency set for speech related participant events for all participants in all connected channels.
	 * \return the current ParticipantSpeakingUpdateRate
	 * \see SetParticipantSpeakingUpdateRate()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
	EVivoxParticipantSpeakingUpdateRate GetParticipantSpeakingUpdateRate() const;

    /**
    * \brief Log the account out of the Vivox system. This will not raise a property event for changing the State to LoginState.LoggedOut, although
    * the state will be set to that upon completion of the function.
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    void Logout();

    /**
    * \brief Get a login token for this account.
    *
    * \param tokenSigningKey the key corresponding to the issuer for this account that is used to sign the token
    * \param tokenExpirationDuration the length of time the token is valid for.
    * \return an access token that can be used to log this account in
    * \remarks To be used only by applications without secure communications requirements.
    */
    UFUNCTION(BlueprintCallable, Category = "vivox")
    FString GetLoginToken(const FString &SecretKey, FTimespan tokenExpirationDuration);

    ILoginSession *VivoxVoiceLoginSession;
};