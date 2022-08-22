
var spotifyPlayerToggled = false;

function hndlToggle(chkBox, togChkId, cacheVarName) {
  console.log("hndlToggle()");
  this[cacheVarName] = chkBox.checked;
  elementFromId = document.getElementById(togChkId);
  if (this[cacheVarName] === true) {
    elementFromId.style.display = "block";
  } else {
    elementFromId.style.display = "none";
  }
}

function auto_height(elem) {  /* javascript */
  elem.style.height = "30px";
  elem.style.height = (elem.scrollHeight) + "px";
}

function toggleGPSOLEDDisplay() {
  $.ajax({
    url: '/ToggledoGPSOLEDDisplay',
    type: 'POST',
    dataType: "json",
    success: function () {
      console.log("toggleGPSOLEDDisplay");
    }
  });
}