var elems = document.getElementsByTagName("canvas");
(function (console) {
    console.save = function (data, filename) {
        if (!data) {
            console.error('Console.save: No data');
            return;
        }
        if (!filename) filename = 'console.json';

        if (typeof data === "object") {
            data = JSON.stringify(data, undefined, 4);
        }

        var blob = new Blob([data], { type: 'text/json' }),
            e = document.createEvent('MouseEvents'),
            a = document.createElement('a');

        a.download = filename;
        a.href = window.URL.createObjectURL(blob);
        a.dataset.downloadurl = ['text/json', a.download, a.href].join(':');
        e.initMouseEvent('click', true, false, window, 0, 0, 0, 0, 0, false, false, false, false, 0, null);
        a.dispatchEvent(e);
    };
})(console);

for (var index = 0; index < elems.length; index++) {
    var element = elems[index];
    var imageDataURL = element.toDataURL("image/png");
    console.save(imageDataURL, index + ".txt");
}
try {
    var index = 0
    var t = setInterval(() => {
        if (index >= elems.length) {
            clearInterval(t)
        }
        if (index % 2 != 0) {
            var element = elems[index];
            var imageDataURL = element.toDataURL("image/png");
            console.save(imageDataURL, index + ".txt");
        }
        index++;

    }, 500);
} catch (error) {
    console.log(error)
}
