/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "VivoxConfig.h"

#include "VivoxClientConfig.generated.h"

UENUM(BlueprintType)
enum class ELogLevel : uint8 {
	NoLogging   UMETA(DisplayName = "no logging"),
	ErrorOnly   UMETA(DisplayName = "error only"),
	Warnings    UMETA(DisplayName = "warnings"),
	Info        UMETA(DisplayName = "info"),
	Debug       UMETA(DisplayName = "debug"),
	Trace       UMETA(DisplayName = "trace"),
	All			UMETA(DisplayName = "all"),
};

UENUM(BlueprintType)
enum class EMediaCodecType : uint8 {
	None		UMETA(DisplayName = "none"),
	Siren14		UMETA(DisplayName = "siren14"),
	Pcmu		UMETA(DisplayName = "pcmu"),
	Nm			UMETA(DisplayName = "nm"),
	Speex		UMETA(DisplayName = "speedx"),
	Siren7		UMETA(DisplayName = "siren7"),
	Opus		UMETA(DisplayName = "opus"),
};

USTRUCT(BlueprintType)
struct FVivoxClientConfig {
	GENERATED_BODY()

	/**
	* \brief Sets the log level of the Vivox SDK.
	* Severity level of logs: -1 = no logging, 0 = errors only, 1 = warnings, 2 = info, 3 = debug, 4 = trace, 5 = log all
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	ELogLevel logLevel = ELogLevel::ErrorOnly;

	/**
	* \brief Set the codec mask that will be used to initialize connector's configured_codecs.
	* codec type none = 0, codec type siren14 = 1, codec type pcmu = 2, codec type nm = 3,
	* codec type speex = 4, codec type siren7 = 5, codec type opus = 6
	* CAUTION: Altering this value will change the quality of the voice experience.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	EMediaCodecType defaultCodecsMask;

	/**
	* \brief Set the Render Source Max Queue Depth.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int renderSourceQueueDepthMax = 20;

	/**
	* \brief Set the Render Source Initial Buffer Count.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int renderSourceInitialBufferCount = 20;

	/**
	* \brief Set the Upstream jitter frame count
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int upstreamJitterFrameCount = 20;

	/**
	* \brief Set the max logins a user can have
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int maxLoginsPerUser = 1;

	/**
	* \brief Set the number of 20 millisecond buffers for the capture device.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int captureDeviceBufferSizeIntervals = 20;

	/**
	* \brief Set the number of 20 millisecond buffers for the render device.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int renderDeviceBufferSizeIntervals = 20;

	/**
	* \brief Set the number of milliseconds to wait before disconnecting audio due to RTP timeout after the call has been established. Zero or negative value turns off the guard (not recommended).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int neverRtpTimeoutMs = 20;

	/**
	* \brief Set the number of milliseconds to wait before disconnecting audio due to RTP timeout after the call has been established. Zero or negative value turns off the guard (not recommended).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int lostRtpTimeoutMs = 20;

	/**
	* \brief Processor Affinity Mask for SDK Threads
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	int64 processorAffinityMask;

	/**
	* \brief Set whether or not to Disable Audio Device Polling Using Timer
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool disableDevicePolling;

	/**
	* \brief Set whether to force capturing silence. Diagnostic purposes only.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool forceCaptureSilence;

	/**
	* \brief Set whether to enable advanced automatic settings of audio levels.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool enableAdvancedAutoLevels;

	/**
	* \brief Enable Persistent Connections (Windows Only) (default: false. True if environment variable "VIVOX_ENABLE_PERSISTENT_HTTP" is set
	* Note that the use of proxies may interfere with behavior controlled by this setting.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool enablePersistentHTTP;

	/**
	* \brief Set whether to disable Audio Ducking. XBox One and iOS.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool disableAudioDucking;

	/**
	* \brief Set whether to enable Fast Network Change Detection. Default of disable.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool enableFastNetworkChangeDetection;

	/**
	* \brief Use Operating System Configured Proxy Settings (Windows Only) (default: 0 or 1 if environment variable "VIVOX_USE_OS_PROXY_SETTINGS" is set)
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool useOSProxySettings;

	/**
	* \brief Allow shared capture devices (shared in the Vivox context only).
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool allowSharedCaptureDevices;

	/**
	* \brief Default of true for most platforms.
	* CAUTION: Altering this value may result in unexpected behavior.
	* For more information, please see our online documentation.  If you have questions, please post in the Vivox Developer Portal forums or submit a support ticket, depending on your support plan.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	bool enableDTX;

public:
	VivoxConfig toVivox() const {
		VivoxConfig vivoxConfig = VivoxConfig();

		switch (logLevel)
		{
		case ELogLevel::NoLogging:
			vivoxConfig.SetLogLevel(vx_log_level::log_none);
			break;
		case ELogLevel::ErrorOnly:
			vivoxConfig.SetLogLevel(vx_log_level::log_error);
			break;
		case ELogLevel::Warnings:
			vivoxConfig.SetLogLevel(vx_log_level::log_warning);
			break;
		case ELogLevel::Info:
			vivoxConfig.SetLogLevel(vx_log_level::log_info);
			break;
		case ELogLevel::Debug:
			vivoxConfig.SetLogLevel(vx_log_level::log_debug);
			break;
		case ELogLevel::Trace:
			vivoxConfig.SetLogLevel(vx_log_level::log_trace);
			break;
		case ELogLevel::All:
			vivoxConfig.SetLogLevel(vx_log_level::log_all);
			break;
		default:
			vivoxConfig.SetLogLevel(vx_log_level::log_error);
			break;
		}

		switch (defaultCodecsMask) {
		case EMediaCodecType::None:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_none);
			break;
		case EMediaCodecType::Siren14:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_siren14);
			break;
		case EMediaCodecType::Pcmu:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_pcmu);
			break;
		case EMediaCodecType::Nm:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_nm);
			break;
		case EMediaCodecType::Speex:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_speex);
			break;
		case EMediaCodecType::Siren7:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_siren7);
			break;
		case EMediaCodecType::Opus:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_opus);
			break;
		default:
			vivoxConfig.SetDefaultCodecsMask(media_codec_type::media_codec_type_none);
			break;
		}

		vivoxConfig.SetRenderSourceQueueDepthMax(renderSourceQueueDepthMax);
		vivoxConfig.SetRenderSourceInitialBufferCount(renderSourceInitialBufferCount);
		vivoxConfig.SetUpstreamJitterFrameCount(upstreamJitterFrameCount);
		vivoxConfig.SetMaxLoginsPerUser(maxLoginsPerUser);
		vivoxConfig.SetCaptureDeviceBufferSizeIntervals(captureDeviceBufferSizeIntervals);
		vivoxConfig.SetRenderDeviceBufferSizeIntervals(renderDeviceBufferSizeIntervals);
		vivoxConfig.SetNeverRtpTimeoutMs(neverRtpTimeoutMs);
		vivoxConfig.SetLostRtpTimeoutMs(lostRtpTimeoutMs);
		vivoxConfig.SetProcessorAffinityMask(processorAffinityMask);
		vivoxConfig.SetDisableDevicePolling(disableDevicePolling);
		vivoxConfig.SetForceCaptureSilence(forceCaptureSilence);
		vivoxConfig.SetEnableAdvancedAutoLevels(enableAdvancedAutoLevels);
		vivoxConfig.SetEnablePersistentHTTP(enablePersistentHTTP);
		vivoxConfig.SetDisableAudioDucking(disableAudioDucking);
		vivoxConfig.SetEnableFastNetworkChangeDetection(enableFastNetworkChangeDetection);
		vivoxConfig.SetUseOSProxySettings(useOSProxySettings);
		vivoxConfig.SetAllowSharedCaptureDevices(allowSharedCaptureDevices);
		vivoxConfig.SetEnableDTX(enableDTX);

		return vivoxConfig;
	}
};