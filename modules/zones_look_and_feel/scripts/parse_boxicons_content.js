const fs = require("fs");

const ToPascalCase = (str) => {
    if (/^[a-z\d]+$/i.test(str)) {
        return str.charAt(0).toUpperCase() + str.slice(1);
    }
    return str.replace(
        /([a-z\d])([a-z\d]*)/gi,
        (g0, g1, g2) => g1.toUpperCase() + g2.toLowerCase()
    ).replace(/[^a-z\d]/gi, '');
}

const BuildGoogleStyleConstant = (str) => {
    return ToPascalCase(`k${ToPascalCase(str)}`);
}


const FillHeaderTemplate = (str) => {
    return `#pragma once
#include <juce_core/juce_core.h>

struct BoxIcons
{
${str}
};
`
}

const FillImplTemplate = (str) => {
    return `#include "BoxIcons.h"
${str}
`
}

let boxicons_content = fs.readFileSync('./boxicons_content.json', 'utf8');
boxicons_content = JSON.parse(boxicons_content);

let header_content = "";
let impl_content = "";

for (let icon in boxicons_content) {
    let icon_variable_name = BuildGoogleStyleConstant(icon);
    let icon_content = boxicons_content[icon];

    header_content += "\n";
    header_content += `static const juce::String ${icon_variable_name};`;

    impl_content += "\n";
    impl_content += `const juce::String BoxIcons::${icon_variable_name} = juce::String::fromUTF8 ("\\u${icon_content}");`;
}

fs.writeFile('./BoxIcons.h', FillHeaderTemplate(header_content), () => {
});
fs.writeFile('./BoxIcons.cpp', FillImplTemplate(impl_content), () => {
});