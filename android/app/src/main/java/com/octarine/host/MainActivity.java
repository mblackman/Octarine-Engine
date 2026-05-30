package com.octarine.host;

import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;

import org.libsdl.app.SDLActivity;

// Thin host activity. Inherits all SDL lifecycle / surface / input handling from SDLActivity;
// the only thing it adds is an Acknowledgements entry on the platform options menu that hands
// off to LicensesActivity. Keeping this as a subclass (not a fork) lets us track upstream SDL
// without merge churn.
public class MainActivity extends SDLActivity {
    private static final int MENU_LICENSES = 1;

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        menu.add(Menu.NONE, MENU_LICENSES, Menu.NONE, "Acknowledgements");
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == MENU_LICENSES) {
            startActivity(new Intent(this, LicensesActivity.class));
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
