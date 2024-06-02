#ifndef FILE_SELECTOR_HPP
#define FILE_SELECTOR_HPP

#include <string>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#else
#include <gtk/gtk.h>
#endif

enum class FileDialogType {
    Open,
    Save
};

class FileSelector {
public:
    static std::string select_file(FileDialogType type, const std::string &title = "Select File") {
        std::string filename;
        
#ifdef _WIN32
        OPENFILENAME ofn;
        char szFile[260] = {0};
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrTitle = title.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (type == FileDialogType::Open) {
            if (GetOpenFileName(&ofn) == TRUE) {
                filename = ofn.lpstrFile;
            }
        } else {
            if (GetSaveFileName(&ofn) == TRUE) {
                filename = ofn.lpstrFile;
            }
        }
#else
        filename = "test.txt";
        return filename;

        gtk_init(0, nullptr);
        GtkWidget *dialog;
        GtkFileChooserAction action = (type == FileDialogType::Open) 
                                      ? GTK_FILE_CHOOSER_ACTION_OPEN 
                                      : GTK_FILE_CHOOSER_ACTION_SAVE;
        dialog = gtk_file_chooser_dialog_new(title.c_str(),
                                             NULL,
                                             action,
                                             ("_Cancel"), GTK_RESPONSE_CANCEL,
                                             ("_Open"), GTK_RESPONSE_ACCEPT,
                                             NULL);
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            char *file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            filename = file;
            g_free(file);
        }
        gtk_widget_destroy(dialog);
#endif

        return filename;
    }
};

#endif // FILE_SELECTOR_HPP
