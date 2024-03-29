# FSBrowserPlus
Extended version of ESP32 example FSBrowser

This enhancement mainly used for my ESP32 remote projects. Please find more details at instructables:

- <https://www.instructables.com/id/Robot-Cat/>

## Depend Libraries
- ESPAsyncWebServer: <https://github.com/me-no-dev/ESPAsyncWebServer.git>
- AsyncTCP: <https://github.com/me-no-dev/AsyncTCP.git>

## Upload Webpage
- Option 1: ESP32 Sketch Data Upload
- Option 2: curl, cd to project data folder then run

    ```
    for file in `ls -A1`; do curl -F "file=@$PWD/$file" fsbrowserPlus.local/edit; done
    ```

## Sample Page
- OV2640 Camera Setting Page: <http://fsbrowserplus.local/ov2640.htm>
- OV3660 Camera Setting Page: <http://fsbrowserplus.local/ov3660.htm>
- OV5640 Camera Setting Page: <http://fsbrowserplus.local/ov5640.htm>
- Camera Snap: <http://fsbrowserplus.local/snap>
- Camera Stream: <http://fsbrowserplus.local:81/stream>
- Web Editor: <http://fsbrowserplus.local/edit>
- WebSocket Tester Page: <http://fsbrowserplus.local/wstester.htm>
- Pose Design Page: <http://fsbrowserplus.local/pose.htm>
- Graph Demo Page: <http://fsbrowserplus.local/graphs.htm>

## Attribute
Arrow vector created by alicia_mb - [www.freepik.com](https://www.freepik.com/vectors/arrow)