Controller.setParameter.connect(this, function(parameter, value) {
    switch (parameter) {
    case Meta.Windshield:
        Controller.parameters.Windshield = value
        break;
    case Meta.FrontTop:
        Controller.parameters.FrontTop = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.FrontMiddle:
        Controller.parameters.FrontMiddle = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.FrontBottom:
        Controller.parameters.FrontBottom = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.RearTop:
        Controller.parameters.RearTop = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.RearMiddle:
        Controller.parameters.RearMiddle = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.RearBottom:
        Controller.parameters.RearBottom = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.FrontDefrost:
        Controller.parameters.FrontDefrost = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.RearDefrost:
        Controller.parameters.RearDefrost = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.AC:
        Controller.parameters.AC = value;
        break;
    case Meta.Recirculate:
        Controller.parameters.Recirculate = value;
        break;
    case Meta.AutoAc:
        Controller.parameters.AutoAc = value;
        break;
    case Meta.Power:
        Controller.parameters.Power = value;
        break;
    case Meta.Fan1:
        Controller.parameters.Fan1 = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.Fan2:
        Controller.parameters.Fan2 = value;
        Controller.parameters.AutoAc = false;
        break;
    case Meta.Temperature1:
        Controller.parameters.Temperature1 = value;
        break;
    case Meta.Temperature2:
        Controller.parameters.Temperature2 = value;
        break;
    case Meta.SeatHeatLeft:
        Controller.parameters.SeatHeatLeft = value;
        break;
    case Meta.SeatHeatRight:
        Controller.parameters.SeatHeatRight = value;
        break;
    }
});

function timerAction() {
    Controller.sendMessage("HVAC Status");
}
