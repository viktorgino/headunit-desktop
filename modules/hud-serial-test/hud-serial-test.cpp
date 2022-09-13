#include "hud-serial-test.h"

HUDSerialTest::HUDSerialTest(QObject *parent) : QObject (parent)
{
    m_pluginSettings.events = QStringList() << "KeyInput" << "MediaInput";
}

void HUDSerialTest::init() {

}

void HUDSerialTest::buttonClicked(QString key) {
    qDebug() << "Button Clicked : " << key;
    if(key == "1"){
    }
    else if(key == "2"){
    }
    else if(key == "3"){
    }
    else if(key == "4"){
    }
    else if(key == "5"){
    }
    else if(key == "6"){
    }
    else if(key == "7"){
    }
    else if(key == "8"){
    }
    else if(key == "9"){
    }
    else if(key == "0"){
    }
    else if(key == "Enter"){
    }
    else if(key == "Exit"){
    }
    else if(key == "Back"){
    }
    else if(key == "Menu"){
    }
    else if(key == "Power"){
    }
    else if(key == "Left"){
    }
    else if(key == "Right"){
    }
    else if(key == "Up"){
    }
    else if(key == "Down"){
    }
    else if(key == "Next"){
        emit message("MediaInput", "Next");
    }
    else if(key == "Previous"){
        emit message("MediaInput", "Previous");
    }
    else if(key == "VolumeUp"){
        emit action("AudioProcessorPlugin::VolumeUp", 0);
    }
    else if(key == "VolumeDown"){
        emit action("AudioProcessorPlugin::VolumeDown", 0);
    }
    else if(key == "TuneUp"){
        emit action("AudioProcessorPlugin::TuneUp", 0);
    }
    else if(key == "TuneDown"){
        emit action("AudioProcessorPlugin::TuneDown", 0);
    }
    else if(key == "Sound"){
        emit action("AudioProcessorPlugin::Sound", 0);
    }
    else if(key == "AMFM"){
    }
    else if(key == "CD"){
    }
    else if(key == "Eject"){
    }
    else if(key == "Auto"){
    }
    else if(key == "Scan"){
    }
}
void HUDSerialTest::setParameter(QString parameter, bool value){
    if(parameter == "TempSelectLeft"){
        m_ccCommandFrame.TempSelectLeft = value;
    } else if(parameter == "TempSelectRight"){
        m_ccCommandFrame.TempSelectRight = value;
    } else if(parameter == "FanSelectLeft"){
        m_ccCommandFrame.FanSelectLeft = value;
    } else if(parameter == "FanSelectRight"){
        m_ccCommandFrame.FanSelectRight = value;
    } else if(parameter == "ProgAuto"){
        m_ccCommandFrame.ProgAuto = value;
    } else if(parameter == "ProgAutoFanFront"){
        m_ccCommandFrame.ProgAutoFanFront = value;
    } else if(parameter == "ProgAutoFanRear"){
        m_ccCommandFrame.ProgAutoFanRear = value;
    } else if(parameter == "ProgWindscreen"){
        m_ccCommandFrame.ProgWindscreen = value;
    } else if(parameter == "Recirculate"){
        m_ccCommandFrame.Recirculate = value;
    } else if(parameter == "RearDefrost"){
        m_ccCommandFrame.RearDefrost = value;
    } else if(parameter == "AC"){
        m_ccCommandFrame.AC = value;
    }
    emit action("HVACPlugin::Update", QVariant::fromValue(m_ccCommandFrame));
}
void HUDSerialTest::setAudioParameter(QString parameter, int value){
    QVariantMap parameterMap;
    parameterMap["key"] = parameter;
    parameterMap["value"] = value;
    emit action("HUDSerialPlugin::AudioControl", parameterMap);
}
void HUDSerialTest::setZoneParameter(QString zone, QString parameter, QVariant value) {
    ClimateControlZoneControl *zoneControl;

    if(zone == "FrontLeft"){
        zoneControl = &m_ccCommandFrame.Front.Left;
    } else if(zone == "FrontRight"){
        zoneControl = &m_ccCommandFrame.Front.Right;
    } else if(zone == "RearLeft"){
        zoneControl = &m_ccCommandFrame.Rear.Left;
    } else if(zone == "RearRight"){
        zoneControl = &m_ccCommandFrame.Rear.Right;
    } else {
        return;
    }

    if(parameter == "Fan"){
        zoneControl->Fan = value.toUInt();
    } else if(parameter == "Temperature"){
        zoneControl->Temperature = value.toUInt();
    }  else if(parameter == "SeatHeating"){
        zoneControl->SeatHeating = value.toUInt();
    } else if(parameter == "DirUp"){
        zoneControl->Direction.Up = value.toBool();
    } else if(parameter == "DirCenter"){
        zoneControl->Direction.Center = value.toBool();
    } else if(parameter == "DirDown"){
        zoneControl->Direction.Down = value.toBool();
    } else if(parameter == "DirAuto"){
        zoneControl->Direction.Auto = value.toBool();
    }
    emit action("HVACPlugin::Update", QVariant::fromValue(m_ccCommandFrame));}


QObject *HUDSerialTest::getContextProperty() {
    return this;
}
