game.BMF_Font = Object.extend({
    file: null,
    init: function(file) {
        var xmlhttp = new XMLHttpRequest();

        if (xmlhttp.overrideMimeType) {
            xmlhttp.overrideMimeType('text/plain');
        }

        xmlhttp.open("GET", file, false);
        xmlhttp.responseType = "text";
        xmlhttp.send();
        
        //now it should be loaded
        
        console.log(xmlhttp.responseText);
        
        
    }
});