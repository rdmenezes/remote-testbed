package remote.gui;
import java.io.File;
import javax.swing.filechooser.*;

/* ImageFilter.java is a 1.4 example used by FileChooserDemo2.java. */
public class FlashImageFilter extends FileFilter {
    public boolean accept(File f) {
        if (f.isDirectory()) {
            return true;
        }

        String extension = getExtension(f);
        if (extension != null) {
            if (extension.equals("exe") || extension.equals("s19") || extension.equals("sky"))
            {
                    return true;
            } else {
                return false;
            }
        }

        return false;
    }

    //The description of this filter
    public String getDescription() {
        return "Flash images (*.exe, *.s19, *.sky)";
    }

    public static String getExtension(File f) {
        String ext = null;
        String s = f.getName();
        int i = s.lastIndexOf('.');

        if (i > 0 &&  i < s.length() - 1) {
            ext = s.substring(i+1).toLowerCase();
        }
        return ext;
    }
}
