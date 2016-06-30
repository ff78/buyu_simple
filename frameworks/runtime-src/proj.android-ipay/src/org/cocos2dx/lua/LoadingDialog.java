package org.cocos2dx.lua;

import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.AnimationDrawable;
import android.view.Gravity;
import android.widget.ImageView;
import android.widget.TextView;
import com.jzgame.standalonebuyu.R;

public class LoadingDialog extends Dialog {
	
	
	private static LoadingDialog customProgressDialog = null;
	
	public LoadingDialog(Context context){
		super(context);
	}
	
	public LoadingDialog(Context context, int theme) {
        super(context, theme);
    }
	
	public static LoadingDialog createDialog(Context context){
		customProgressDialog = new LoadingDialog(context, R.style.CustomProgressDialog);
		customProgressDialog.setContentView(R.layout.loading_dialog_style);
		customProgressDialog.getWindow().getAttributes().gravity = Gravity.CENTER;
		customProgressDialog.setCanceledOnTouchOutside(false);
		
		return customProgressDialog;
	}
 
    public void onWindowFocusChanged(boolean hasFocus){
    	
    	if (customProgressDialog == null){
    		return;
    	}
    	
        ImageView imageView = (ImageView) customProgressDialog.findViewById(R.id.loadingImageView);
        AnimationDrawable animationDrawable = (AnimationDrawable) imageView.getBackground();
        animationDrawable.start();
    }
 
    public LoadingDialog setTitile(String strTitle){
    	return customProgressDialog;
    }
    
    public LoadingDialog setMessage(String strMessage){

    	TextView tvMsg = (TextView)customProgressDialog.findViewById(R.id.id_tv_loadingmsg);
    	
    	if (tvMsg != null){
    		tvMsg.setText(strMessage);
    	}
    	
    	return customProgressDialog;
    }
}
