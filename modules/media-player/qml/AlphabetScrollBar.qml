import QtQuick 2.11
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    id: scroll_bar
    property var alphabet : "...,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z".split(",");
    property int current_indx: 10
    property int letter_height: height/alphabet.length;
    property var letters : [];
    property int count: 0;
    property string firstLetter: "A"
    property string lastLetter: "Z"
    property string topItemFirstLetter: "..."
    property string bottomItemFirstLetter: "..."
    property int fontSize: letter_height!=0?letter_height *0.8:5
    signal positionViewAtIndex(int index, var mode);
    onLettersChanged: {
        if(typeof(letters) !== "undefined"){
            if(letters.length > 0){
                firstLetter = Object.keys(letters)[0];
                lastLetter = Object.keys(letters)[Object.keys(letters).length-1];
            }
        }
    }

    Rectangle{
        opacity: 0
        height: letter_height
        width:parent.width
        id: scroller
    }
    Rectangle{
        color:"grey"
        //radius: width/2
        width:parent.width
        height: parent.height/alphabet.length
        opacity: 1 + (scroller.x/10)
        y:{
            var topLetterId = scroll_bar.alphabet.indexOf(topItemFirstLetter);
            var bottomLetterId = scroll_bar.alphabet.indexOf(bottomItemFirstLetter);
            var diff = bottomLetterId - topLetterId+1;
            if(topLetterId > 0 & bottomLetterId > 0){
                height=scroller.height * diff;
                return topLetterId * letter_height;
            }
            return 0;
        }
    }

    Column {
        anchors.fill: parent
        id:scroll_column
        spacing:0
        Repeater{
            model:alphabet
            id:scroll_repeater
            Text{
                //Layout.fillHeight: true
                height:letter_height
                property int indx: index
                x : {
                    var diff;
                    if(index > current_indx){
                        diff = current_indx - index
                    } else if(index === current_indx) {
                        diff = 0;
                    } else {
                        diff =  current_indx - index
                    };
                    if(diff > 7 || diff < -7){
                        return 0;
                    } else {
                        var angle = 0 - (90/7 * diff);
                        var ret =  scroller.x * Math.cos(angle*(Math.PI / 180));
                        return ret;
                    }
                }
                Behavior on x {
                    NumberAnimation { duration: 100}
                }
                width: parent.width
                text:scroll_bar.alphabet[index]
                font.pointSize: fontSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color:{
                    var topLetterId = scroll_bar.alphabet.indexOf(topItemFirstLetter);
                    var bottomLetterId = scroll_bar.alphabet.indexOf(bottomItemFirstLetter);
                    var diff = bottomLetterId - topLetterId+1;
                    if(topLetterId <= index & bottomLetterId >= index){
                        return "#64B5F6";
                    }
                    return "white";
                }
            }
        }
    }
    MouseArea {
        id:scroll_bar_mouse
        anchors.fill: parent
        propagateComposedEvents: false
        preventStealing: false
        property bool isMouseDown: false
        function getIndex(y){
            return Math.floor(y/letter_height);
        }
        function scrollToY(y){
            scroller.y=y
            current_indx = getIndex(y);
            var letter = alphabet[current_indx];
            if(current_indx > alphabet.indexOf(lastLetter)){
                positionViewAtIndex(count-1,ListView.End);
            } else if(current_indx < alphabet.indexOf(firstLetter)){
                positionViewAtIndex(0,ListView.Beginning);
            } else if(typeof(letters[letter]) === "undefined") {
                for(var letter_item in letters){
                    if(current_indx<alphabet.indexOf(letter_item)){
                        positionViewAtIndex(letters[letter_item],ListView.Beginning);
                        break;
                    }
                }
            } else {
                positionViewAtIndex(letters[letter],ListView.Beginning);
            }

        }

        onPressed: {
            isMouseDown=true;
            scrollToY(mouse.y);
        }
        onReleased: {
            isMouseDown=false;
            scroller.x=0
        }
        onCanceled: {
            isMouseDown=false;
        }
        onPositionChanged: {
            if(mouse.y <= height & mouse.y >= 0){
                scrollToY(mouse.y);
            }
            if(mouse.x >= -100 & mouse.x <= 0){
                scroller.x=mouse.x
            }
        }
        onClicked: {
        }
    }
}
