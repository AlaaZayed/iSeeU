package com.eaaa.iseeu;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import com.eaaa.iseeu.R;

import android.app.Activity;
import android.graphics.Point;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Menu;
import android.view.Window;
import android.view.WindowManager;

public class MeDetectionActivity extends Activity implements
		CvCameraViewListener {
	

	private static final String TAG = "iSeeU";
	private CameraBridgeViewBase cameraView;

	// Loading OpenCV Library
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				Log.i(TAG, "OpenCV loaded successfully");

				// Load native library after OpenCV initialization
				System.loadLibrary("micro_expression_detection");
				
				cameraView.enableView();
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};

	 @Override
	    public void onCreate(Bundle savedInstanceState) {
	        Log.i(TAG, "called onCreate");
	        super.onCreate(savedInstanceState);
	        //window preparation
	        requestWindowFeature(Window.FEATURE_NO_TITLE);
	        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

	        setContentView(R.layout.camera_surface);

	        //JavaCameraView object
	        cameraView = (CameraBridgeViewBase) findViewById(R.id.surface_view);
	        cameraView.setCvCameraViewListener(this);
	    }

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	@Override
    public void onPause()
    {
        if (cameraView != null)
            cameraView.disableView();
        super.onPause();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        cameraView.disableView();
    }

  

	// Camera Interface Methods
	@Override
	public void onCameraViewStarted(int width, int height) {
		// TODO Auto-generated method stub
		Log.i(TAG, "Camera started");
		MicroExpRecogNative.train();
	}

	@Override
	public void onCameraViewStopped() {
		// TODO Auto-generated method stub

	}

	@Override
	public Mat onCameraFrame(Mat inputFrame) {
		// TODO Auto-generated method stub
		Log.i(TAG, "Operating on frame");
		MicroExpRecogNative.recognizeExpression(inputFrame.getNativeObjAddr());
		return inputFrame;
	}

}
