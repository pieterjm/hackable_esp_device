// Get elements
let btn;
let checkbox;
let rect;
let circ1;
let circ2;

let on_off;

main();
function main() {
    loadState();
    loadBrightness();
}

function loadState() {
    var request =  new XMLHttpRequest();
    request.open("GET", "/state", true);
    request.setRequestHeader("Content-Type",
      "application/x-www-form-urlencoded");

    request.onreadystatechange = function() {
      var done = 4, ok = 200;
      if (request.readyState == done && request.status == ok) {
        if (request.responseText) {
            updateState(request.responseText);
        }
      }
    };
    request.send();
}

function loadBrightness() {
    var request =  new XMLHttpRequest();
    request.open("GET", "/brightness", true);
    request.setRequestHeader("Content-Type",
      "application/x-www-form-urlencoded");

    request.onreadystatechange = function() {
      var done = 4, ok = 200;
      if (request.readyState == done && request.status == ok) {
        if (request.responseText) {
            updateBrightness(request.responseText);
        }
      }
    };
    request.send();
}

var updateBrightness = function(text) {
    document.getElementById("brightness").value = text;
}

var updateState = function(text) {
    if (text == "1") {
        on_off = true;
    } else {
        on_off = false;
    }

    document.getElementById("cb_power").checked = on_off;
}

//Update brightness
$("#brightness").change(
    function() {
        var brightness = $(this).val();
        $.ajax({
            url: "/set_brightness",
            type: "get",
            data: {
                brightness: brightness
                },    
            success: function(response) {
                //$("#place_for_suggestions").html(response);
            },
            error: function(xhr) {
                //alert("Error: " + xhr);
            }
        });
    }
);

function loadPage() {
    document.location.reload(false);         //Reload page with current scrollindex
}

function setPower() {
    on_off = document.getElementById("cb_power").checked ? 1 : 0;
    
    $.ajax({
        url: "/set_power",
        type: "get",
        data: {
            state: on_off
            },
        success: function(response) {
            //$("#place_for_suggestions").html(response);
        },
        error: function(xhr) {
            //alert("Error: " + xhr);
        }
    });
}