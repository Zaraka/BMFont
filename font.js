game.BMF_Font = Object.extend({
    file: null,
    content: null,
    lineHeight: null,
    sizeOver: null,
    sizeUnder: null,
    addSpace: null,
    sizeInner: null,
    colors_count: null,
    colors: null,
    info: null,
    charactersNum: null,
    characters: null,
    init: function(font) {
        this.file = font;

        var array = me.loader.getArray(font);
        if(array === null){
            throw "Font " + font + " has not been found, or it's empty file";
            return false;
        }
        
        if(array[0] !== 225 && array[1] !== 230 && array[2] !== 213 && array[3] !== 26){
            throw "Font magic header doesn't match!";
            return false;
        }
        
        if(array[4] !== 17){
            throw "Font version mismatch";
            return false;
        }

        this.lineHeight = array[5];
        this.sizeOver = array[6];
        this.sizeUnder = array[7];
        this.addSpace = array[8];
        this.sizeInner = array[9];

        this.colors_count = array[16];
        var colorLength = this.colors_count * 3;
        this.colors = new Array(this.colors_count);
        this.colors[0] = ("transparent");
        for (var color = 0; color < this.colors_count; color++) {
            var r, g, b;
            r = array[17 + (color * 3) + 0] * 4;
            g = array[17 + (color * 3) + 1] * 4;
            b = array[17 + (color * 3) + 2] * 4;
            this.colors[color + 1] = ("rgb(" + r + "," + g + "," + b + ")");
        }

        var infoLength = array[17 + colorLength];
        this.info = "";
        for (var letter = 18 + colorLength; letter < (18 + colorLength + infoLength); letter++) {
            this.info += String.fromCharCode(array[letter]);
        }
        this.charactersNum = array[18 + colorLength + infoLength] + (array[19 + colorLength + infoLength] * 255);
        this.characters = [];
        var charStart = 20 + colorLength + infoLength;
        for (var char = 0; char < this.charactersNum; char++) {
            var character, width, height, relx, rely, shift, canvas;
            character = String.fromCharCode(array[charStart]);
            width = array[charStart + 1];
            height = array[charStart + 2];
            relx = array[charStart + 3];
            rely = array[charStart + 4];
            shift = array[charStart + 5];
            if (width === 0 || height === 0) {
                canvas = me.video.createCanvas(width + shift, height + this.lineHeight);
            } else {
                canvas = me.video.createCanvas(width, height);
            }

            charStart = charStart + 6;
            var context = canvas.getContext('2d');
            for (var tmpHeight = 0; tmpHeight < height; tmpHeight++) {
                for (var tmpWidth = 0; tmpWidth < width; tmpWidth++) {
                    var color = this.colors[array[charStart]];
                    if (color !== "transparent") {
                        context.fillStyle = color;
                        context.fillRect(tmpWidth, tmpHeight, 1, 1);
                    }
                    charStart++;
                }
            }
            //charStart++;
            this.characters[character] = new game.BMF_Char(character, width, height, relx, rely, shift, canvas);
        }
    },
    draw: function(context, text, x, y) {
        var localX = x;
        for (var ch = 0; ch < text.length; ch++) {
            var character = this.characters[text[ch]];
            if(typeof character === "undefined"){
                throw("Character '" + text[ch] + "' isnt in font " + this.file + "!");
                continue
            }
            context.drawImage(character.canvas, localX + character.relx, y + character.rely);
            localX += character.shift;
        }
    }
});

game.BMF_Char = Object.extend({
    char: null,
    width: null,
    height: null,
    relx: null,
    rely: null,
    shift: null,
    canvas: null,
    init: function(char, width, height, relx, rely, shift, canvas) {
        this.char = char;
        this.width = width;
        this.height = height;
        this.relx = relx;
        this.rely = rely;
        this.shift = shift;
        this.canvas = canvas;
    }
});