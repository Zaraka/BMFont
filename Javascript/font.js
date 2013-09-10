game.BMF_Font = Object.extend({
    file: null,
    content: null,
    init: function(file) {
        this.file = file;
        console.log(file);
        
        var blob = me.loader.getBlob("arakis");
        console.log(blob);
    }
});