package com.oseasy.mmc.multiclient;

import android.app.IntentService;
import android.content.Intent;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */

public class MultiClientServer extends IntentService {
    private static final String ACTION_CORE = "com.oseasy.mmc.multiclient.action.CORE";

    public MultiClientServer() {
        super("MultiRedirectionServer");
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_CORE.equals(action)) {
                handleActionCore();
            }
        }
    }

    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private void handleActionCore() {
        // TODO: Handle action Foo
        Core core = new Core(this.getApplicationContext());
        core.Run();
    }
}