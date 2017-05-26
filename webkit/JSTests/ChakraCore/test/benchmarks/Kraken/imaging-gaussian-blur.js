/*
 Copyright (C) 2007 Apple Inc.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

if(typeof(WScript) === "undefined")
{
    var WScript = {
        Echo: print
    }
}

function record(time) {
    document.getElementById("console").innerHTML = time + "ms";
    if (window.parent) {
        parent.recordResult(time);
    }
}


var sigma = 10; // radius
var kernel, kernelSize, kernelSum;

function buildKernel() {
   var ss = sigma * sigma;
   var factor = 2 * Math.PI * ss;
   kernel = [];
   kernel.push([]);
   var i = 0, j;

   do {
       var g = Math.exp(-(i * i) / (2 * ss)) / factor;
       if (g < 1e-3) break;
       kernel[0].push(g);
       ++i;
   } while (i < 7);

   kernelSize = i;
   for (j = 1; j < kernelSize; ++j) {
       kernel.push([]);
       for (i = 0; i < kernelSize; ++i) {
           var g = Math.exp(-(i * i + j * j) / (2 * ss)) / factor;
           kernel[j].push(g);
       }
   }
   kernelSum = 0;
   for (j = 1 - kernelSize; j < kernelSize; ++j) {
       for (i = 1 - kernelSize; i < kernelSize; ++i) {
           kernelSum += kernel[Math.abs(j)][Math.abs(i)];
       }
   }
}

buildKernel();

var width = 400, height = 267;


var _sunSpiderStartDate = new Date();

//print("i: " + i + "j: " + j);

function gaussianBlur() {
    for (var y = 0; y < height; ++y) {
        for (var x = 0; x < width; ++x) {
            var r = 0, g = 0, b = 0, a = 0;
            for (var j = 1 - kernelSize; j < kernelSize; ++j) {
                if (y + j < 0 || y + j >= height) continue;
                for (var i = 1 - kernelSize; i < kernelSize; ++i) {
                    if (x + i < 0 || x + i >= width) continue;
                    r += squidImageData[4 * ((y + j) * width + (x + i)) + 0] * kernel[Math.abs(j)][Math.abs(i)];
                    g += squidImageData[4 * ((y + j) * width + (x + i)) + 1] * kernel[Math.abs(j)][Math.abs(i)];
                    b += squidImageData[4 * ((y + j) * width + (x + i)) + 2] * kernel[Math.abs(j)][Math.abs(i)];
                    a += squidImageData[4 * ((y + j) * width + (x + i)) + 3] * kernel[Math.abs(j)][Math.abs(i)];
            }
            }
            squidImageData[4 * (y * width + x) + 0] = r / kernelSum;
            squidImageData[4 * (y * width + x) + 1] = g / kernelSum;
            squidImageData[4 * (y * width + x) + 2] = b / kernelSum;
            squidImageData[4 * (y * width + x) + 3] = a / kernelSum;
    }
    }
    return squidImageData;
}
gaussianBlur();

var _sunSpiderInterval = new Date() - _sunSpiderStartDate;

WScript.Echo("### TIME:", _sunSpiderInterval, "ms");