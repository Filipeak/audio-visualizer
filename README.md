# Audio Visualizer


## Overview
This project is an audio visualizer that creates dynamic visual representations of audio input in real-time. It uses Fast Fourier Transform (FFT) to analyze the frequency components of the audio signal and generates visual effects based on the audio data.


## Requirements
- C Language (MinGW or any C compiler)
- CMake
- Ninja Build System (preferably)
- SDL3 Library Installed


## Run
Clone the repository: ```git clone --recurse-submodules https://github.com/Filipeak/audio-visualizer```

Build the project: ```mkdir build && cd build && cmake -G Ninja .. && ninja```

Run the executable: ```./audio-visualizer <mp3 file path>```


## Preview
![](https://github.com/Filipeak/audio-visualizer/blob/main/assets/preview.png)


## Next Steps
- Optimize FFT (iterative version)
- Smoothen fft plots between frames