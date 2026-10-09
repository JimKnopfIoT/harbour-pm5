import QtQuick 2.0

// Deep-space backdrop. The base vertical gradient and the two radial "nebulae"
// are computed in one fragment shader and dithered by ~1 LSB per pixel, so the
// dark gradients show a smooth transition instead of 8-bit banding. Drawn
// straight to the window (no cached 8-bit layer that would re-quantize it); the
// scene is otherwise idle, so this repaints only when something changes.
Item {
    anchors.fill: parent

    ShaderEffect {
        anchors.fill: parent
        property vector2d resolution: Qt.vector2d(width, height)

        fragmentShader: "
            uniform lowp float qt_Opacity;
            varying highp vec2 qt_TexCoord0;
            uniform highp vec2 resolution;

            // 3-stop radial nebula with a transparent outer stop, composited
            // over bg (straight-alpha source-over), scaled by op.
            lowp vec3 nebula(lowp vec3 bg, highp vec2 uv, highp vec2 center,
                             highp vec2 rad, lowp vec3 cA, lowp vec3 cB, lowp float op) {
                highp float d = length((uv - center) / rad);
                lowp vec3 col; lowp float a;
                if (d < 0.5) { col = mix(cA, cB, d / 0.5); a = 1.0; }
                else if (d < 1.0) { lowp float t = (d - 0.5) / 0.5;
                                    col = mix(cB, vec3(0.0), t); a = 1.0 - t; }
                else { col = vec3(0.0); a = 0.0; }
                return mix(bg, col, a * op);
            }

            highp float hash(highp vec2 p) {
                p = fract(p * vec2(443.897, 441.423));
                p += dot(p, p.yx + 19.19);
                return fract((p.x + p.y) * p.x);
            }

            void main() {
                highp vec2 uv = qt_TexCoord0;
                highp float aspect = resolution.x / resolution.y;

                // base vertical gradient (#232429 -> #171820 -> #0d0e12)
                lowp vec3 c0 = vec3(0.13725, 0.14118, 0.16078);
                lowp vec3 c1 = vec3(0.09020, 0.09412, 0.12549);
                lowp vec3 c2 = vec3(0.05098, 0.05490, 0.07059);
                lowp vec3 col = (uv.y < 0.55)
                    ? mix(c0, c1, uv.y / 0.55)
                    : mix(c1, c2, (uv.y - 0.55) / 0.45);

                // orange nebula, lower-left
                col = nebula(col, uv, vec2(0.20, 0.92), vec2(0.9, 0.9 * aspect),
                             vec3(1.0, 0.47843, 0.0), vec3(0.2, 0.08235, 0.0), 0.26);
                // green nebula, upper-right
                col = nebula(col, uv, vec2(0.86, 0.16), vec2(0.8, 0.8 * aspect),
                             vec3(0.15686, 0.78039, 0.43529), vec3(0.03922, 0.2, 0.12549), 0.20);

                // triangular-PDF dither, ~+/-1 LSB, breaks the 8-bit banding
                highp vec2 px = uv * resolution;
                highp float n = hash(px) + hash(px + vec2(17.0, 0.0)) - 1.0;
                col += n * (1.0 / 255.0);

                gl_FragColor = vec4(col, 1.0) * qt_Opacity;
            }
        "
    }

    // static stars (no animation -> scene stays idle)
    Repeater {
        model: 70
        delegate: Rectangle {
            width: Math.random() < 0.85 ? 1 : 2
            height: width
            radius: width / 2
            color: "#ffffff"
            opacity: 0.12 + Math.random() * 0.5
            x: Math.random() * parent.width
            y: Math.random() * parent.height
        }
    }
}
