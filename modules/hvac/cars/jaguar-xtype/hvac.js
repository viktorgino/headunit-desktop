var temperature = 20;
var fanLevel = 1;
var tempUnit = 0;
var recirculate = false;
var hvacOn = false;
var frontDefrost = false;
var rearDefrost = false;
var acOn = false;
var autoAc = false;
var airflowDir = 1;
var fanAuto = false;
var dirAuto = false;
var freshAir = false;

HVACGui.toggleWindshield.connect(this,function(){setAirflowDirection(1);});
HVACGui.toggleTop.connect(this,function(){setAirflowDirection(2);});
HVACGui.toggleMiddle.connect(this,function(){setAirflowDirection(3);});
HVACGui.toggleBottom.connect(this,function(){setAirflowDirection(4);});
HVACGui.setFan1.connect(this,function(param){setFanLevel(param);});
HVACGui.setTemp1.connect(this,function(param){setTemperature(param);});
HVACGui.toggleRecirculate.connect(this,function(){toggleRecirculate();});
HVACGui.toggleAutoAc.connect(this,function(){toggleAutoAc();});
HVACGui.toggleAc.connect(this,function(){toggleAc();});
HVACGui.toggleFrontDefrost.connect(this,function(){toggleFrontDefrost();});
HVACGui.toggleRearDefrost.connect(this,function(){toggleRearDefrost();});
HVACGui.togglePower.connect(this,function(){togglePower();});


//Not available in the car, sample implementation

//HVACGui.setLeftSeatHeat.connect(this,function(param){console.log("setLeftSeatHeat : "+param);});
//HVACGui.setRightSeatHeat.connect(this,function(param){console.log("setRightSeatHeatChanged : "+param);});
//HVACGui.setFan2.connect(this,function(param){console.log("setFan2 : "+param);});
//HVACGui.setTemp2.connect(this,function(param){console.log("setTemp2 : "+param);});

convert = {
  bin2dec : function(s){return parseInt(s, 2).toString(10)},
  bin2hex : function(s){return parseInt(s, 2).toString(16)},
  dec2bin : function(s){return parseInt(s, 10).toString(2)},
  dec2hex : function(s){return parseInt(s, 10).toString(16)},
  dec2hex2: function(s){return ("00" + parseInt(s, 10).toString(16)).substr(-2)},
  hex2bin : function(s){return parseInt(s, 16).toString(2)},
  hex2dec : function(s){return parseInt(s, 16).toString(10)}
};

function setTemperature(temp) {
    temperature = (temp - 17) / 0.5
}

function setFanLevel(level){
    if(level >= 0 && level < 8){
        fanLevel = level;
        fanAuto = false;
        HVACGui.fanAuto = false;
    }
    if(level === 8){
        fanAuto = true;
        HVACGui.fanAuto = true;
    }
}

function setTempUnit(unit) {
    tempUnit = unit;
}

function toggleRecirculate() {
    recirculate = !recirculate;
    HVACGui.recirculateActive = recirculate;
}

function togglePower() {
    hvacOn = !hvacOn;
    HVACGui.powerActive = hvacOn;
}

function toggleRearDefrost() {
    rearDefrost = !rearDefrost;
    HVACGui.rearDefActive = rearDefrost;
}
function toggleFrontDefrost() {
    if(!frontDefrost)
        setAirflowDirection(1);
    else
        setAirflowDirection(5);
    frontDefrost = !frontDefrost;
    HVACGui.frontDefActive = frontDefrost;
}

function toggleAc() {
    acOn = !acOn;
    HVACGui.acActive = acOn;
}
function toggleAutoAc() {
    autoAc = !autoAc;
    HVACGui.autoAcActive = autoAc;
}

function setAirflowDirection(dir) {
    switch(dir)
    {
        case 1://Windshield
            HVACGui.windshieldActive = !HVACGui.windshieldActive;
            HVACGui.topActive = false;
            HVACGui.middleActive = false;
            HVACGui.autoActive = false;
            dirAuto = false;
            break;
        case 2://Top
            HVACGui.windshieldActive = false;
            HVACGui.topActive = !HVACGui.topActive;
            HVACGui.middleActive = false;
            HVACGui.autoActive = false;
            dirAuto = false;
            break;
        case 3://Middle
            HVACGui.windshieldActive = false;
            HVACGui.topActive = false;
            HVACGui.middleActive = !HVACGui.middleActive;
            HVACGui.bottomActive = false;
            HVACGui.autoActive = false;
            dirAuto = false;
            break;
        case 4://Bottom
            HVACGui.middleActive = false;
            HVACGui.bottomActive = !HVACGui.bottomActive;
            HVACGui.autoActive = false;
            dirAuto = false;
            break;
        case 5://Auto
            HVACGui.windshieldActive = false;
            HVACGui.topActive = false;
            HVACGui.middleActive = false;
            HVACGui.bottomActive = false;
            HVACGui.autoActive = !HVACGui.autoActive;
            dirAuto = true;
            break;
    }
    if(HVACGui.windshieldActive && HVACGui.bottomActive){
        airflowDir = 4;
    } else if(HVACGui.topActive && HVACGui.bottomActive){
        airflowDir = 2;
    } else if(HVACGui.topActive){
        airflowDir = 1;
    } else if(HVACGui.bottomActive){
        airflowDir = 3;
    } else if(HVACGui.autoActive){
        airflowDir = 6;
    } else if(HVACGui.windshieldActive){
        airflowDir = 5;
    } else {
        airflowDir = 1;
    }
}

function timerAction() {
    var byte2 = (Boolean(fanAuto)?15:fanLevel) | (Boolean(fanAuto) || Boolean(dirAuto)) << 4 | (Boolean(recirculate)?3:0) << 4 | Boolean(freshAir && !hvacOn) << 6;
    var byte3 = temperature | airflowDir << 5 | Boolean(dirAuto) << 7;
    var byte4 = temperature | Boolean(acOn) << 5 | Boolean(tempUnit) << 7;
    var byte5 = 0 | Boolean(rearDefrost) << 1 | Boolean(hvacOn) << 6;

    var byte1 = "03";
    var message = "A1 8E 68 " + "03 " + convert.dec2hex2(byte2) + " " + convert.dec2hex2(byte3) + " " + convert.dec2hex2(byte4) + " " + convert.dec2hex2(byte5);
    HVACGui.sendMessage(message);
}
