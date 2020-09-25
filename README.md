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
- Camera Setting Page: <http://fsbrowserplus.local/ov2640.htm>
- Camera Snap: <http://fsbrowserplus.local/snap>
- Camera Stream: <http://fsbrowserplus.local/stream>
- Web Editor: <http://fsbrowserplus.local/edit>
- WebSocket Tester Page: <http://fsbrowserplus.local/wstester.htm>
- Pose Design Page: <http://fsbrowserplus.local/pose.htm>
- Graph Demo Page: <http://fsbrowserplus.local/graphs.htm>
