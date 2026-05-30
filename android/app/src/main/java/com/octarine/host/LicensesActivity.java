package com.octarine.host;

import android.app.Activity;
import android.graphics.Typeface;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.ScrollView;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

// Renders THIRD_PARTY_LICENSES.txt (bundled under the APK's assets/ by app/build.gradle's
// licenseAssetsDir wiring) inside a scrollable, selectable TextView. No layout XML — building
// the view tree programmatically keeps this self-contained and avoids a new res/layout file.
public class LicensesActivity extends Activity {
    private static final String LICENSES_ASSET = "THIRD_PARTY_LICENSES.txt";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTitle("Acknowledgements");

        TextView text = new TextView(this);
        text.setTextIsSelectable(true);
        text.setTypeface(Typeface.MONOSPACE);
        int pad = (int) (16 * getResources().getDisplayMetrics().density);
        text.setPadding(pad, pad, pad, pad);
        text.setText(loadAsset());

        ScrollView scroll = new ScrollView(this);
        scroll.addView(text, new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        setContentView(scroll, new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
    }

    private CharSequence loadAsset() {
        StringBuilder out = new StringBuilder();
        try (InputStream in = getAssets().open(LICENSES_ASSET);
             BufferedReader r = new BufferedReader(
                     new InputStreamReader(in, StandardCharsets.UTF_8))) {
            String line;
            while ((line = r.readLine()) != null) {
                out.append(line).append('\n');
            }
        } catch (IOException e) {
            return "Failed to load " + LICENSES_ASSET + ": " + e.getMessage();
        }
        return out;
    }
}
