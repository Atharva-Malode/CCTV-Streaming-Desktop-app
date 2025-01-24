#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace dnn;
using namespace std;

int main()
{
    // Load YOLOv11 ONNX model
    String modelWeights = "yolov11.onnx"; // Path to YOLOv11 ONNX model
    String classesFile = "coco.names";    // COCO class labels

    // Load class labels
    vector<string> classes;
    ifstream ifs(classesFile);
    string line;
    while (getline(ifs, line)) classes.push_back(line);

    // Load the network
    Net net = readNetFromONNX(modelWeights);
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);

    // Initialize video capture
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Couldn't open camera" << endl;
        return -1;
    }

    Mat frame;
    float confidenceThreshold = 0.5;
    float nmsThreshold = 0.4;

    while (cap.read(frame)) {
        if (frame.empty()) {
            cerr << "Error: Blank frame" << endl;
            break;
        }

        // Prepare input blob
        Mat blob;
        blobFromImage(frame, blob, 1/255.0, Size(640, 640), Scalar(), true, false);
        
        // Set input and forward pass
        net.setInput(blob);
        vector<Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        // Process outputs
        vector<Rect> boxes;
        vector<float> confidences;
        vector<int> classIds;
        
        // YOLOv11 output parsing (assuming single output tensor)
        Mat output = outputs[0];
        int rows = output.size[2];
        int dimensions = output.size[1];

        // Reshape to (rows, dimensions)
        output = output.reshape(1, rows);

        float* data = (float*)output.data;
        for (int i = 0; i < rows; ++i) {
            float* detection = &data[i * dimensions];
            
            // Get class scores and find max confidence
            Mat scores(1, classes.size(), CV_32F, detection + 4);
            Point classId;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classId);

            if (confidence > confidenceThreshold) {
                // Scale box coordinates to original frame
                float x_scale = frame.cols / 640.0f;
                float y_scale = frame.rows / 640.0f;

                int cx = static_cast<int>(detection[0] * x_scale);
                int cy = static_cast<int>(detection[1] * y_scale);
                int w = static_cast<int>(detection[2] * x_scale);
                int h = static_cast<int>(detection[3] * y_scale);

                // Convert to rectangle coordinates
                int left = cx - w/2;
                int top = cy - h/2;

                boxes.push_back(Rect(left, top, w, h));
                confidences.push_back(confidence);
                classIds.push_back(classId.x);
            }
        }

        // Apply Non-Maximum Suppression
        vector<int> indices;
        NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

        // Draw results
        for (int idx : indices) {
            Rect box = boxes[idx];
            rectangle(frame, box, Scalar(0, 255, 0), 2);
            string label = format("%s: %.2f", classes[classIds[idx]].c_str(), confidences[idx]);
            putText(frame, label, Point(box.x, box.y - 10), 
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
        }

        // Show output
        imshow("YOLOv11 Real-time Detection", frame);
        if (waitKey(1) == 27) break; // ESC to exit
    }

    cap.release();
    destroyAllWindows();
    return 0;
}