
$(document).ready(function () {
  $("#subTaskToggles #toggleSub #GPSOLEDPRNTToggle").checked = getGPSOLEDDisplay();
  $("#doPanTilt #doPanTiltForm #panContainer #pan").value = getCurrentPan();
  $("#doPanTilt #doPanTiltForm #tiltContainer #tilt").value = getCurrentTilt();


  let baseEnabled = localStorage.getItem('baseEnabled');
  let baseTiltEnabled = localStorage.getItem('baseTiltEnabled');
  let elbowEnabled = localStorage.getItem('elbowEnabled');
  let elevateWristEnabled = localStorage.getItem('elevateWristEnabled');
  let wristRotateEnabled = localStorage.getItem('wristRotateEnabled');
  let clawEnabled = localStorage.getItem('clawEnabled');
  $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseToggle").prop("checked", baseEnabled !== null ? baseEnabled : false);
  $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseTiltToggle").prop("checked", baseTiltEnabled !== null ? baseTiltEnabled : false);
  $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseToggle").prop("checked", elbowEnabled !== null ? elbowEnabled : false);
  $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseToggle").prop("checked", elevateWristEnabled !== null ? elevateWristEnabled : false);
  $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseToggle").prop("checked", wristRotateEnabled !== null ? wristRotateEnabled : false);
  $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseToggle").prop("checked", clawEnabled !== null ? clawEnabled : false);


});

function hndlToggle(chkBox, togChkId, cacheVarName) {
  console.log("hndlToggle -> chkBox.checked: " + chkBox.checked);
  elementFromId = document.getElementById(togChkId);
  elementFromId.style.display = (chkBox.checked === true) ? "block" : "none";
}


function auto_height(elem) {  /* javascript */
  elem.style.height = "30px";
  elem.style.height = (elem.scrollHeight) + "px";
}


function toggleocvDNNAnalysis(chkBox, togChkId, cacheVarName) {
  $.ajax({
    url: '/toggleocvDNNAnalysis',
    type: 'POST',
    dataType: "json",
    success: function (toggleocvDNNAnalysisData) {
      if (GPSOLEDDisplayData.length > 0) {
        console.log("toggleocvDNNAnalysisData" + JSON.stringify(toggleocvDNNAnalysisData));
        localStorage.setItem(cacheVarName, toggleocvDNNAnalysisData["nowState"]);

        elementFromId = document.getElementById(togChkId);
        elementFromId.style.display = (chkBox.checked === true) ? "block" : "none";
      }
    }
  });
}

//GPSOLEDDisplay----------------------------------------------------------------

function toggleGPSOLEDDisplay() {
  $.ajax({
    url: '/toggleGPSOLEDDisplay',
    type: 'POST',
    dataType: "json",
    success: function (GPSOLEDDisplayData) {
      if (GPSOLEDDisplayData.length > 0) {
        console.log("GPSOLEDDisplayData" + JSON.stringify(GPSOLEDDisplayData));
        localStorage.setItem('GPSOLEDDisplayState', GPSOLEDDisplayData["nowState"]);
        $("#subTaskToggles #toggleSub #GPSOLEDPRNTToggle").checked = localStorage.getItem('GPSOLEDDisplayState')
      }
    }
  });
}


function getGPSOLEDDisplay() {
  if (localStorage.getItem('GPSOLEDDisplayState') === undefined) {
    $.ajax({
      url: '/getGPSOLEDDisplay',
      type: 'GET',
      dataType: "json",
      success: function (GPSOLEDDisplayData) {
        if (GPSOLEDDisplayData.length > 0) {
          console.log("getGPSOLEDDisplay" + JSON.stringify(GPSOLEDDisplayData));
          localStorage.setItem('GPSOLEDDisplayState', GPSOLEDDisplayData["nowState"]);
          return localStorage.getItem('GPSOLEDDisplayState')
        }
      }
    });
  } else {
    return localStorage.getItem('GPSOLEDDisplayState');
  }
}
function onValueRangeChange(value, labelId, prefix, affix) {
  document.querySelector(labelId).innerHTML = (prefix + value + affix);
}
//----------------------------------------------------------------

//doPanTilt----------------------------------------------------------------

$(document).ready(function () {

  $("#doPanTilt #doPanTiltForm").bind('click touchend', function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/doPanTilt',
      data: $(this).serialize(),
      success: function (getCurrentPanData) {
        if (getCurrentPanData.length > 0) {
          localStorage.setItem('CurrentPan', getCurrentPanData["CurrentPan"]);
          localStorage.setItem('CurrentTilt', getCurrentPanData["CurrentTilt"]);
        }
      }
    });

  });
});

function getCurrentPan() {
  if (localStorage.getItem('CurrentPan') === undefined) {
    $.ajax({
      url: '/getCurrentPan',
      type: 'GET',
      dataType: "json",
      success: function (getCurrentPanData) {
        if (getCurrentPanData.length > 0) {
          console.log("getCurrentPanData" + JSON.stringify(getCurrentPanData));
          localStorage.setItem('CurrentPan', getCurrentPanData[0]["CurrentPan"]);
          return localStorage.getItem('CurrentPan');
        }
      }
    });
  } else {
    return localStorage.getItem('CurrentPan');
  }
}
function getCurrentTilt() {
  if (localStorage.getItem('CurrentTilt') === undefined) {
    $.ajax({
      url: '/getCurrentTilt',
      type: 'GET',
      dataType: "json",
      success: function (getCurrentTiltData) {
        if (getCurrentTiltData.length > 0) {
          console.log("getCurrentTiltData" + JSON.stringify(getCurrentTiltData));
          console.log("CurrentTilt: " + getCurrentTiltData[0]["CurrentTilt"])
          localStorage.setItem('CurrentTilt', getCurrentTiltData[0]["CurrentTilt"]);
          return localStorage.getItem('CurrentTilt');
        }
      }
    });
  } else {
    return localStorage.getItem('CurrentTilt');
  }
}
//----------------------------------------------------------------

//textToSpeech----------------------------------------------------------------
$(document).ready(function () {
  $("#textToSpeech #textToSpeechForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/textToSpeech',
      data: $(this).serialize()
    });

  });
});

//----------------------------------------------------------------
//micRecAndPlayback----------------------------------------------------------------
$(document).ready(function () {
  $("#micRecAndPlayback #micRecForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh
    audio = $("#micRecAndPlayback #micRecPlaybackOnWebForm #audio")
    audio.attr("src", null);

    $.ajax({
      type: "POST",
      url: '/startRecording',
      data: $(this).serialize()
    });

  });
});


function dolocalPlayback(e) {
  e.preventDefault(); // avoid page refresh
  $.ajax({
    url: '/doLatestPlaybackOnPlatform'
  });
}



// function getRecordingDeNoised (e) {
//   e.preventDefault(); // avoid page refresh
//   audio = $("#micRecAndPlayback #micRecPlaybackOnWebForm #audio")
//   audio.attr("src", window.location.origin + '/getRecording_denoised');
// }

function getRecordingRaw (e) {
    e.preventDefault(); // avoid page refresh
    audio = $("#micRecAndPlayback #micRecPlaybackOnWebForm #audio")
    audio.attr("src", window.location.origin + '/getRecording_raw');
  }
//----------------------------------------------------------------



//oLEDDisplayTxt----------------------------------------------------------------
$(document).ready(function () {
  $("#oLEDDisplayTxt #oledDisplayTextForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/writeOLEDText',
      data: $(this).serialize()
    });

  });
});

//----------------------------------------------------------------

//ultraSonicReading----------------------------------------------------------------
$(document).ready(function () {
  $("#ultraSonicReading #ultraSonicReadingForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/ultraSonicRequest',
      dataType: "json",
      success: function (ultrasonic_data) {
        if (ultrasonic_data.length > 0) {
          //console.log("ultrasonic_data" + JSON.stringify(ultrasonic_data));
          //
          //console.log("UltraSonicDistance: " + ultrasonic_data[0]["UltraSonicDistance"])
          dist = ultrasonic_data[0]["UltraSonicDistance"].replace("/r", "");
          temp = ultrasonic_data[0]["UltraSonicTemp"].replace("/r", "").replace("c", "");
          $("#ultraSonicReading #ultraSonicReadingForm #ftextDistance").val(dist);
          $("#ultraSonicReading #ultraSonicReadingForm #ftextTemp").val(temp);
        }
      }
    });

  });
});

function toggleOLEDUltraSonicDisplay() {
  $.ajax({
    url: '/toggleOLEDUltraSonicDisplay',
    type: 'POST',
    dataType: "json",
  });
  console.log("toggleOLEDUltraSonicDisplay")
}

//----------------------------------------------------------------
//AccMagReading----------------------------------------------------------------

function toggleOLEDAccMagDisplay() {
  $.ajax({
    url: '/toggleOLEDAccMagDisplay',
    type: 'POST',
    dataType: "json",
  });
  con; sole.log("toggleOLEDAccMagDisplay")
}


$(document).ready(function () {
  $("#accMagReading #accMagReadingForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/accMagRequest',
      dataType: "json",
      success: function (accMag_data) {
        if (accMag_data.length > 0) {
          console.log("accMag_data" + JSON.stringify(accMag_data));
          //

          acc = accMag_data[0]["AccMagAcc"].replace("/r", "");
          mag = accMag_data[0]["AccMagMag"].replace("/r", "");
          heading = accMag_data[0]["AccMagHeading"].replace("/r", "");

          console.log("acc:" + acc)
          console.log("mag:" + mag)
          console.log("heading:" + heading)
          $("#accMagReading #accMagReadingForm #ftextAccel").val(acc);
          $("#accMagReading #accMagReadingForm #ftextMag").val(mag);
          $("#accMagReading #accMagReadingForm #ftextHeading").val(heading);
        }
      }
    });

  });
});

//----------------------------------------------------------------
//4WD control
//FRAorFRB
function toggleFRA() {
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRBToggle").removeAttr('checked');
};
function toggleFRB() {
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRAToggle").removeAttr('checked');
};
//FLAorFLB
function toggleFLA() {
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLBToggle").removeAttr('checked');
};
function toggleFLB() {
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLAToggle").removeAttr('checked');
};
//BRAorBRB
function toggleBRA() {
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRBToggle").removeAttr('checked');
};
function toggleBRB() {
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRAToggle").removeAttr('checked');
};
//BLAorBLB
function toggleBLA() {
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLBToggle").removeAttr('checked');
};
function toggleBLB() {
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLAToggle").removeAttr('checked');
};
function ResetAll() {
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRBToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRAToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLBToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLAToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRBToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRAToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLBToggle").removeAttr('checked');
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLAToggle").removeAttr('checked');
};

function Forwards() {
  ResetAll();
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRAToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLAToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRAToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLAToggle").prop("checked", true);
};

function Backwards() {
  ResetAll();
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRBToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLBToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRBToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLBToggle").prop("checked", true);
};

function RotLeft() {
  ResetAll();
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRBToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLAToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRBToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLAToggle").prop("checked", true);
};

function RotRight() {
  ResetAll();
  $("#FourWDControl #FourWDControlForm #FRAorFRB #FRAToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #FLAorFLB #FLBToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BRAorBRB #BRAToggle").prop("checked", true);
  $("#FourWDControl #FourWDControlForm #BLAorBLB #BLBToggle").prop("checked", true);
};




$(document).ready(function () {
  $("#FourWDControl #FourWDControlForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh
    //
    var motorArr = [];
    if ($("#FourWDControl #FourWDControlForm #FRAorFRB #FRBToggle").prop("checked") === true) {

      motorArr.push('FRB')
    }
    if ($("#FourWDControl #FourWDControlForm #FRAorFRB #FRAToggle").prop("checked") === true) {
      motorArr.push('FRA')
    }
    if ($("#FourWDControl #FourWDControlForm #FLAorFLB #FLBToggle").prop("checked") === true) {
      motorArr.push('FLB')
    }
    if ($("#FourWDControl #FourWDControlForm #FLAorFLB #FLAToggle").prop("checked") === true) {
      motorArr.push('FLA')
    }
    if ($("#FourWDControl #FourWDControlForm #BRAorBRB #BRBToggle").prop("checked") === true) {
      motorArr.push('BRB')
    }
    if ($("#FourWDControl #FourWDControlForm #BRAorBRB #BRAToggle").prop("checked") === true) {
      motorArr.push('BRA')
    }
    if ($("#FourWDControl #FourWDControlForm #BLAorBLB #BLBToggle").prop("checked") === true) {
      motorArr.push('BLB')
    }
    if ($("#FourWDControl #FourWDControlForm #BLAorBLB #BLAToggle").prop("checked") === true) {
      motorArr.push('BLA')
    }

    var cmdStr = {
      duration: $("#FourWDControl #FourWDControlForm #duratationContainer #duration").val(),
      speed: $("#FourWDControl #FourWDControlForm #speedContainer #speed").val(),
      motors: motorArr
    }
    console.log(cmdStr);
    var payload = JSON.stringify(cmdStr);


    $.ajax({
      type: "POST",
      url: '/FourWheeledDriveRequest',
      contentType: "application/json; charset=utf-8",
      data: payload,
      success: function (respBuiltCommand) {
        console.log("Processed 4WD Command: " + respBuiltCommand)
      }
    });

  });
})

//----------------------------------------------------------------
//6 Degreees of freedom Arm
$(document).ready(function () {
  $("#DOF6ArmControl #DOF6ArmControlForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh
    //
    //
    cmdStr = {
      base: {
        enabled: $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #BaseToggle").prop("checked") === true,
        angle: $("#DOF6ArmControl #DOF6ArmControlForm #BaseContainer #base").val()
      },
      baseTilt: {
        enabled: $("#DOF6ArmControl #DOF6ArmControlForm #BaseTiltContainer #BaseTiltToggle").prop("checked") === true,
        angle: $("#DOF6ArmControl #DOF6ArmControlForm #BaseTiltContainer #baseTilt").val()
      },
      elbow: {
        enabled: $("#DOF6ArmControl #DOF6ArmControlForm #ElbowContainer #ElbowAngToggle").prop("checked") === true,
        angle: $("#DOF6ArmControl #DOF6ArmControlForm #ElbowContainer #elbow").val()
      },
      wristElavate: {
        enabled: $("#DOF6ArmControl #DOF6ArmControlForm #WristElevateContainer #WristElavateToggle").prop("checked") === true,
        angle: $("#DOF6ArmControl #DOF6ArmControlForm #WristElevateContainer #wristElavate").val()
      },
      wristRotate: {
        enabled: $("#DOF6ArmControl #DOF6ArmControlForm #WristRotateContainer #WristRotateToggle").prop("checked") === true,
        angle: $("#DOF6ArmControl #DOF6ArmControlForm #WristRotateContainer #wristRotate").val()
      },
      claw: {
        enabled: $("#DOF6ArmControl #DOF6ArmControlForm #ClawContainer #clawToggle").prop("checked") === true,
        angle: $("#DOF6ArmControl #DOF6ArmControlForm #ClawContainer #claw").val()
      }

    }
    console.log(cmdStr);
    var payload = JSON.stringify(cmdStr);

    $.ajax({
      type: "POST",
      url: '/do6DOFARMCmd',
      contentType: "application/json; charset=utf-8",
      data: payload,
      success: function (respBuiltCommand) {
        console.log("Processed 6DOFARM Command: " + respBuiltCommand)
      }
    });

  });
});

//----------------------------------------------------------------

