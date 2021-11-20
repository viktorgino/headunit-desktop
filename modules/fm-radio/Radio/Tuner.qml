import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0



Item {
    id: tuner
    property int freqWidth: 100
    property int freqRangeStart: 77
    property int freqRangeEnd: 118
    property double initialFreq: 88.8
    property double freqRealRangeStart: 87.5
    property double freqRealRangeEnd: 108
    property double frequency: 88.8

    signal changed(double frequency)

    function slideXtoFreq(freq){
        return 0 - ((freq-freqRangeStart)*freqWidth-tuner.width/2)-2;
    }
    function freqToSlideX(freq){
        return 0 - ((freq-freqRangeStart)*freqWidth-tuner.width/2)-2;
    }
    function moveToFreq(freq){
        if(freq >= freqRealRangeStart && freq <= freqRealRangeEnd){
            tuneSlider.x = slideXtoFreq(freq);
            slideUpdate();
        }
    }
    function moveFreqWith(amount){
        var freq = getFreq() + 1 * (amount/Math.abs(amount));
        moveToFreq(freq);
    }
    function getFreq(){
        return -((tuneSlider.x  - tuner.width/2 + 2)/freqWidth - freqRangeStart)
    }
    function slideUpdate(){
        frequency = getFreq();
        tuner.changed(frequency)
    }

    FontLoader{id:ralewayRegular; source:"qrc:/qml/fonts/Raleway-Regular.ttf"}

    Item {
        id: tuneSlider
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.1
        width: (freqRangeEnd-freqRangeStart)*freqWidth
        x: slideXtoFreq(initialFreq)
        RowLayout {
            anchors.bottomMargin: 3
            anchors.topMargin: 3
            spacing: 0
            anchors.fill: parent
            Repeater{
                model: freqRangeEnd-freqRangeStart
                Item {
                    Layout.fillHeight: true
                    width:freqWidth

                    function calcHeight(index){
                        switch(index){
                        case 0:
                            return 20;
                        case 5:
                            return 10;
                        default:
                            return 5;
                        }
                    }
                    function calcWidth(index){
                        switch(index){
                        case 0:
                            return 3;
                        default:
                            return 1;
                        }
                    }
                    function outterWidth(index){
                        switch(index){
                        case 0:
                            return freqWidth/10+1;
                        case 9:
                            return freqWidth/10-1;
                        default:
                            return freqWidth/10;
                        }
                    }
                    Row {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        Repeater{
                            model:10
                            Item{
                                id: item3
                                height:20
                                width:outterWidth(index)
                                Rectangle {
                                    anchors.top: parent.top
                                    anchors.topMargin: 0
                                    height:calcHeight(index)
                                    width:calcWidth(index)
                                    color:"#ffffff"
                                }
                            }
                        }
                    }
                    Row {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        Repeater{
                            model: 10
                            Item{
                                id: item2
                                height:20
                                width:outterWidth(index)
                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 0
                                    height:calcHeight(index)
                                    width:calcWidth(index)
                                    color:"#ffffff"
                                }
                            }
                        }
                    }
                    Text {
                        text: freqRangeStart + index
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: width/-2
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 24
                        font.family: ralewayRegular.name
                        color:"#ffffff"
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        drag.target: tuneSlider;
        drag.axis: "XAxis"
        drag.maximumX: (freqRangeStart-freqRealRangeStart)*freqWidth+parent.width/2-2
        drag.minimumX: drag.maximumX - (freqRealRangeEnd - freqRealRangeStart)*100
        onPositionChanged:slideUpdate()
        onWheel: moveFreqWith(wheel.angleDelta.y)
    }

    Rectangle {
        width: 5
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#00000000" }
            GradientStop { position: 0.5; color: "#d50000" }
            GradientStop { position: 1.0; color: "#00000000" }

        }
    }
}
