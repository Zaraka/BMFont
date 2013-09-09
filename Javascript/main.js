me.state.CHARACTER_PICK = me.state.USER;

var game =
        {
            screenWidth: 400,
            screenHeight: 220,
            guiLayer: 15,
            weapons: {},
            consumables: {},
            armors: {},
            items: {},
            gui: {},
            fonts: {},
            destroyable: {},
            mechanic: {},
            instances: {},
            entities: {},
            object_layer: 4,
            effects: {},
            npcs: {},
            audio: {},
            pathfinding: {},
            spells: {},
            LAYERS: {GUI: 15, NPC: 5, ITEMS: 4, TOP: 20},
            version: "0.0.5",
            onload: function()
            {
                if (!me.video.init('screen', this.screenWidth, this.screenHeight, true, 2.0, true)) {
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
              
                me.sys.fps = 30;


                // start the game
                me.state.change(me.state.MENU);
            }
        };



window.onReady(function() {
    game.onload();
});