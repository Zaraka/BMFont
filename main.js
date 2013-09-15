
var game =
        {
            onload: function()
            {
                if (!me.video.init('screen', 800, 400, true, 1.0, true)) {
                    alert("Sorry but your browser does not support html 5 canvas.");
                    return;
                }

                if (!(window.File && window.FileReader && window.FileList && window.Blob)) {
                    alert('The File APIs are not fully supported in this browser.');
                    return;
                }

                me.audio.init("mp3,ogg");
                me.loader.onload = this.loaded.bind(this);
                me.loader.preload(game.resources);
                me.state.change(me.state.LOADING);
            },
            /* ---callback when everything is loaded---*/
            loaded: function()
            {
                // set the "Play/Ingame" Screen Object
                me.state.set(me.state.PLAY, new game.PlayScreen());

                // start the game
                me.state.change(me.state.PLAY);
            }
        };

game.PlayScreen = me.ScreenObject.extend({
    init: function() {
        this.arakis = new game.BMF_Font("arakis");
        this.bloody = new game.BMF_Font("bloody");
        this.dns_1 = new game.BMF_Font("dns_anker-2");
        this.dns_2 = new game.BMF_Font("dns_gate-1");
        this.robocop = new game.BMF_Font("robocop");
        this.oldfrenc = new game.BMF_Font("oldfrenc");
        this.jetpack = new game.BMF_Font("jetpack");
        this.parent(true);
    },
    draw: function(context) {
        me.video.clearSurface(context, "black");
        this.arakis.draw(context, "HELLO WORLD", 50, 50);
        this.bloody.draw(context, "HELLO WORLD", 50, 70);
        this.dns_1.draw(context, "HELLO WORLD", 50, 90);
        this.dns_2.draw(context, "HELLO WORLD", 50, 110);
        this.robocop.draw(context, "HELLO WORLD", 50, 150);
        this.oldfrenc.draw(context, "HELLO WORLD", 50, 170);
        this.jetpack.draw(context, "HELLO", 50, 190);
    },
    onDestroyEvent: function() {
        this.arakis = null;
        this.bloody = null;
        this.dns_1 = null;
        this.dns_2 = null;
    }
});

window.onReady(function() {
    game.onload();
});