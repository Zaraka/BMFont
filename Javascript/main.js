
var game =
        {
            onload: function()
            {
                if (!me.video.init('screen', 400, 200, true, 2.0, true)) {
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
        this.font = null;
    },
    onResetEvent: function() {
        this.font = new game.BMF_Font("assets/arakis10.bmf");
        me.game.add(this.font);
        me.game.sort();
    },
    onDestroyEvent: function() {
        me.game.remove(this.font);
        this.font = null;
    }
});

window.onReady(function() {
    game.onload();
});