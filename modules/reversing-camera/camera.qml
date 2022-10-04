import QtQuick 2.0
import QtMultimedia 5.15
import HUDTheme 1.0

ThemeRoot {

    // Camera {
    //     id: camera

    //     imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

    //     exposure {
    //         exposureCompensation: -1.0
    //         exposureMode: Camera.ExposurePortrait
    //     }

    //     flash.mode: Camera.FlashRedEyeReduction

    //     imageCapture {
    //         onImageCaptured: {
    //             photoPreview.source = preview  // Show the preview in an Image
    //         }
    //     }
    // }

    // VideoOutput {
    //     source: camera
    //     anchors.fill: parent
    //     focus : visible // to receive focus and capture key events when visible
    // }

    VideoOutput {
        anchors.fill: parent
        source: pluginContext
    }
}