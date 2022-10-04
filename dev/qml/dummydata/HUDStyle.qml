import QtQuick 2.0

QtObject {
    property var colors: {
        "text": "#ffffff",
        "subText": "#CACACA",
        "headingText1": "#ffffff",
        "headingText2": "#ffffff",
        "headingText3": "#ffffff",
        "headingText4": "#ffffff",
        "headingText5": "#ffffff",
        "icon": "#ffffff",
        "iconActive": "#ff5722",
        "box": "#212121",
        "formBox": "#3f51b5",
        "formText": "#ffffff",
        "formSubText": "#aaaaaa",
        "formBackground": "#80000000",
        "formPopupBackground": "#646464"
    }

    property var sizes: {
        "text": 11,
        "subText": 10,
        "headingText1": 12,
        "headingText2": 14,
        "headingText3": 16,
        "headingText4": 18,
        "headingText5": 20,
        "formText": 11,
        "formSubText": 10,
        "menuText": 10,
        "menuIcon": 30,
        "bottomBarHeight": 100
    }

    property QtObject background: QtObject {
        property string objectName: ""
        property string color: "#ffffff"
        property bool image: true
        property string imagePath: "backgrounds/micah-hallahan-14075-unsplash.jpg"
        property bool slideshow: false
        property string slideshowPath: ""
        property int slideshowTime: 0
    }

    property var themeSettings: {
        "objectName": "",
        "font": "Raleway"
    }

    property var settings: [{
            "items": [{
                    "defaultValue": "#ffffff",
                    "label": "UI text",
                    "name": "text",
                    "type": "color"
                }, {
                    "defaultValue": "#CACACA",
                    "label": "UI sub text",
                    "name": "subText",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Heading Level 1",
                    "name": "headingText1",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Heading Level 2",
                    "name": "headingText2",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Heading Level 3",
                    "name": "headingText3",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Heading Level 4",
                    "name": "headingText4",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Heading Level 5",
                    "name": "headingText5",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Icon",
                    "name": "icon",
                    "type": "color"
                }, {
                    "defaultValue": "#ff5722",
                    "label": "Active icon",
                    "name": "iconActive",
                    "type": "color"
                }, {
                    "defaultValue": "#212121",
                    "label": "Main UI box",
                    "name": "box",
                    "type": "color"
                }, {
                    "defaultValue": "#3f51b5",
                    "label": "Form box",
                    "name": "formBox",
                    "type": "color"
                }, {
                    "defaultValue": "#ffffff",
                    "label": "Form text",
                    "name": "formText",
                    "type": "color"
                }, {
                    "defaultValue": "#aaaaaa",
                    "label": "Form sub text",
                    "name": "formSubText",
                    "type": "color"
                }, {
                    "defaultValue": "#80000000",
                    "label": "Form background",
                    "name": "formBackground",
                    "type": "color"
                }, {
                    "defaultValue": "#646464",
                    "label": "Popup background",
                    "name": "formPopupBackground",
                    "type": "color"
                }],
            "label": "Colors",
            "name": "colors",
            "type": "items"
        }, {
            "items": [{
                    "defaultValue": 11,
                    "label": "UI text",
                    "name": "text",
                    "type": "tumbler"
                }, {
                    "defaultValue": 10,
                    "label": "UI sub text",
                    "name": "subText",
                    "type": "tumbler"
                }, {
                    "defaultValue": 12,
                    "label": "Heading Level 1",
                    "name": "headingText1",
                    "type": "tumbler"
                }, {
                    "defaultValue": 14,
                    "label": "Heading Level 2",
                    "name": "headingText2",
                    "type": "tumbler"
                }, {
                    "defaultValue": 16,
                    "label": "Heading Level 3",
                    "name": "headingText3",
                    "type": "tumbler"
                }, {
                    "defaultValue": 18,
                    "label": "Heading Level 4",
                    "name": "headingText4",
                    "type": "tumbler"
                }, {
                    "defaultValue": 20,
                    "label": "Heading Level 5",
                    "name": "headingText5",
                    "type": "tumbler"
                }, {
                    "defaultValue": 11,
                    "label": "Form text",
                    "name": "formText",
                    "type": "tumbler"
                }, {
                    "defaultValue": 10,
                    "label": "Form sub text",
                    "name": "formSubText",
                    "type": "tumbler"
                }, {
                    "defaultValue": 10,
                    "label": "Side menu text",
                    "name": "menuText",
                    "type": "tumbler"
                }, {
                    "defaultValue": 30,
                    "label": "Side menu icon",
                    "name": "menuIcon",
                    "type": "tumbler"
                }, {
                    "defaultValue": 100,
                    "label": "Bottom Bar height",
                    "name": "bottomBarHeight",
                    "type": "tumbler"
                }],
            "label": "Sizes",
            "name": "sizes",
            "type": "items"
        }, {
            "items": [{
                    "defaultValue": true,
                    "label": "Type",
                    "name": "image",
                    "textOff": "Color",
                    "textOn": "Image",
                    "type": "switch"
                }, {
                    "conditionTarget": "image",
                    "conditionValue": false,
                    "conditional": true,
                    "defaultValue": "#ffffff",
                    "label": "Background",
                    "name": "color",
                    "type": "color"
                }, {
                    "conditionTarget": "image",
                    "conditionValue": true,
                    "conditional": true,
                    "defaultValue": false,
                    "label": "Type",
                    "name": "slideshow",
                    "textOff": "Image",
                    "textOn": "Slideshow",
                    "type": "switch"
                }, {
                    "conditionTarget": "slideshow",
                    "conditionValue": true,
                    "conditional": true,
                    "defaultVtruealue": 60,
                    "label": "Slide show time",
                    "maximum": 120,
                    "minimum": 10,
                    "name": "slideshowTime",
                    "stepSize": 1,
                    "suffix": "secs",
                    "type": "tumbler"
                }, {
                    "browserType": "folder",
                    "conditionTarget": "slideshow",
                    "conditionValue": true,
                    "conditional": true,
                    "label": "Select folder",
                    "name": "slideshowPath",
                    "type": "file"
                }, {
                    "browserType": "file",
                    "conditionTarget": "slideshow",
                    "conditionValue": false,
                    "conditional": true,
                    "defaultValue": "backgrounds/micah-hallahan-14075-unsplash.jpg",
                    "label": "Select image",
                    "name": "imagePath",
                    "nameFilters": ["*.png", "*.jpg"],
                    "type": "file"
                }],
            "label": "Background Settings",
            "name": "background",
            "type": "items"
        }, {
            "items": [{
                    "defaultValue": "Raleway",
                    "label": "Font",
                    "name": "font",
                    "textType": "string",
                    "type": "textfield"
                }],
            "label": "Theme Settings",
            "name": "themeSettings",
            "type": "items"
        }]
}
