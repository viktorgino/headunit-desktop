import com.navit.graphics_qt5 1.0
import QtQuick 2.9
import QtQuick.Window 2.2
import com.navit.gui 1.0

Window {
    visible: true
	width: 800
	height: 480

    Connections {
        target: NavitGUI
        ignoreUnknownSignals: true
        onHideMenu: {
            hideMainMenu()
        }
        onDisplayMenu: {
            mainMenu.submenu = source
            mainMenu.state = 'visible'
            console.log("showing menu with submenu " + mainMenu.submenu)
            mainMenu.source = '' // Needed when switching submenus from the code to refresh the menu
            mainMenu.source = "menu.qml"
        }
    }

	function hideMainMenu(){
		mainMenu.source = ''
		mainMenu.state = 'default'
	}

	color: "black"
	id: container

	QNavitQuick {
		id: navit1
		width: parent.width
		height: parent.height
		// focus: true
		opacity: 0;
		Component.onCompleted: {
            console.log(width + "x" + height)
			navit1.opacity = 1;
		}
		Behavior on opacity {
			NumberAnimation {
				id: opacityAnimation;duration: 1000;alwaysRunToEnd: true
			}
		}
	}

	Loader {
		id: mainMenu
		width: parent.width
		height: parent.height
		x: parent.width
		opacity: 0
		property string submenu

		states: [
			State {
				name: "visible"
				PropertyChanges {
					target: mainMenu
					x: 0
					opacity: 1
				}
			}
		]
		transitions: [
			Transition {
				NumberAnimation {
					properties: "x,y,opacity";duration: 200
				}
				onRunningChanged: {
					if ((mainMenu.opacity == 0) && (!running)){
                        NavitGUI.resize(navit1.width, navit1.height);
						console.log("transition done, hiding menu window size " + navit1.width + " x " + navit1.height)
					}
				}
			}
		]
	}
}
