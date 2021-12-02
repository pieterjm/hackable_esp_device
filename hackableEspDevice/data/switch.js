// Get elements
let btn;
let checkbox;
let rect;
let circ1;
let circ2;

let on_off;

$(document).ready(function(){

    if (document.getElementById("on_off").value == 1) {
        on_off = true;
    } else {
        on_off = false;
    }

    document.getElementById("cb_power").checked = on_off;
});

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