#ifndef TGCALLS_VIDEO_CAPTURE_INTERFACE_IMPL_H
#define TGCALLS_VIDEO_CAPTURE_INTERFACE_IMPL_H

#include "VideoCaptureInterface.h"
#include <memory>
#include "ThreadLocalObject.h"
#include "api/media_stream_interface.h"
#include "platform/PlatformInterface.h"

namespace tgcalls {

class VideoCapturerInterface;
class Threads;

class VideoCaptureInterfaceObject {
public:
	VideoCaptureInterfaceObject(std::string deviceId, std::shared_ptr<PlatformContext> platformContext, Threads &threads);
	~VideoCaptureInterfaceObject();

	void switchToDevice(std::string deviceId);
	void setState(VideoState state);
    void setPreferredAspectRatio(float aspectRatio);
	void setOutput(std::shared_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> sink);
	void setStateUpdated(std::function<void(VideoState)> stateUpdated);
    void setRotationUpdated(std::function<void(int)> rotationUpdated);
    void setOnFatalError(std::function<void()> error);
	webrtc::VideoTrackSourceInterface *source();
    int getRotation();

private:
    void updateAspectRateAdaptation();
    
    rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> _videoSource;
	std::shared_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> _currentUncroppedSink;
	std::shared_ptr<PlatformContext> _platformContext;
    std::pair<int, int> _videoCapturerResolution;
	std::unique_ptr<VideoCapturerInterface> _videoCapturer;
	std::function<void(VideoState)> _stateUpdated;
    std::function<void()> _onFatalError;
    std::function<void(int)> _rotationUpdated;
	VideoState _state = VideoState::Active;
    float _preferredAspectRatio = 0.0f;
    bool _shouldBeAdaptedToReceiverAspectRate = true;
};

class VideoCaptureInterfaceImpl : public VideoCaptureInterface {
public:
	VideoCaptureInterfaceImpl(std::string deviceId, std::shared_ptr<PlatformContext> platformContext, std::shared_ptr<Threads> threads);
	virtual ~VideoCaptureInterfaceImpl();

	void switchToDevice(std::string deviceId) override;
	void setState(VideoState state) override;
    void setPreferredAspectRatio(float aspectRatio) override;
	void setOutput(std::shared_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> sink) override;
    void setOnFatalError(std::function<void()> error) override;


	ThreadLocalObject<VideoCaptureInterfaceObject> *object();

private:
	ThreadLocalObject<VideoCaptureInterfaceObject> _impl;

};

} // namespace tgcalls

#endif
