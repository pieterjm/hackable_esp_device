
main();
function main() {

}

//Set alarm times
$("#loginBtn").click(
    function() {
        var username = document.getElementById("username").value;
        var password = document.getElementById("password").value;
       
        $.ajax({
            url: "/login",
            type: "get",
            data: {
                username: username,
                password: password,
                cycle_dur: cycleDur
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