const details = {
    flex: {
        title: "Flex Sensors",
        desc: "High-precision variable resistors that detect the degree of finger curvature. These act as the interface between physical gesture and digital data."
    },
    esp32: {
        title: "ESP32 Controller",
        desc: "The processing powerhouse. It runs the conversion algorithms, handles sensor input, and commands the audio module to trigger specific words."
    },
    mpu: {
        title: "Motion Tracking",
        desc: "Using the MPU6050, Signet understands its position in 3D space, allowing the system to differentiate between vertical and horizontal gestures."
    }
};

function showDetail(part) {
    const box = document.getElementById('detail-box');
    box.style.opacity = 0;
    setTimeout(() => {
        document.getElementById('part-title').innerText = details[part].title;
        document.getElementById('part-desc').innerText = details[part].desc;
        box.style.opacity = 1;
    }, 200);
}

function updateSimulator() {
    const rFlex = document.getElementById('resistor-slider').value;
    const vIn = 3.3; 
    const fixedR = 10000; 
    
    // Vout = Vin * (R / (R + Rflex))
    const vOut = vIn * (fixedR / (parseInt(fixedR) + parseInt(rFlex)));
    
    document.getElementById('r-val').innerText = (rFlex/1000).toFixed(1) + "k Ω";
    document.getElementById('v-out').innerText = vOut.toFixed(2) + " V";
}
