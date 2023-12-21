function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}
function downloadStringAsFile(content, fileName, mimeType) {
    var blob = new Blob([content], { type: mimeType });
    var a = document.createElement('a');
    a.href = window.URL.createObjectURL(blob);
    a.download = fileName;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
}

// 使用方式
// var myString = "Hello, world!";
// downloadStringAsFile(myString, "example.txt", "text/plain");

var pageImgBox1 = document.getElementById('pageImgBox1');
var pageImgHeight = pageImgBox1.clientHeight + 20 // 有 20 是下方 margin

var scrollElement = document.getElementById('scroll');
var pageboxElement = document.getElementById('pagebox');
var content = ""
var pageImages = document.getElementsByClassName("page-lmg")
// for (let index = 0; index < 10; index++) {
let index = 827
var textname = index + "-"
for (; index < pageImages.length; index++) {
    var element = pageImages[index];
    scrollElement.scrollTop = pageImgHeight * index
    var retry = 0
    while (element.getElementsByTagName("img").length <= 0) {
        await sleep(5000);
        retry++
        if (retry > 3) {
            break
        }
    }
    if (retry > 3) {
        break
    }
    content += `<div id="${index}">\n`
    content += element.innerHTML
    content += "</div>\n"

}
textname += ((index - 1) + ".txt")
downloadStringAsFile(content, textname, "text/plain");


index = 0
for (; index < pageImages.length; index++) {
    var element = pageImages[index];
    scrollElement.scrollTop = pageImgHeight * index
    var retry = 0
    while (element.getElementsByTagName("img").length <= 0) {
        await sleep(1000);
        retry++
        if (retry > 3) {
            break
        }
    }
    if (retry > 3) {
        break
    }
}
console.log(index)