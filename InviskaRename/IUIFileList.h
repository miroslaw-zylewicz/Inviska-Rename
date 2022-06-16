﻿#ifndef IUIFileList_h
#define IUIFileList_h

#include <QSplitter>
#include <QDir>
#include <QFileSystemWatcher>
#include <QFileIconProvider>
#include <QActionGroup>
#include <QStack>
#include <QStyledItemDelegate>
#include "IRenameInvalidCharSub.h"
#include "ISysFileInfoSort.h"
class QTableWidget;
class QTableWidgetItem;
class QMenu;
class IUIMainWindow;
class IUIMenuBar;
class IUIToolBar;
class IUIRename;


class IUIFileList : public QSplitter
{
    Q_OBJECT
    friend class PreviewTableHighlightDelegate;

private:
    // Main window
    IUIMainWindow*              m_pmwMainWindow;

    // For reading directory contents and monitoring for changes in directory
    QDir                        m_qdirDirReader;
    QFileSystemWatcher          m_qfswFSWatcher;

    // QDir filter set to either show or hide hidden and system files depending on user preferences
    QDir::Filters               m_qdirfHiddenFileFilter;

    // For sorting file list using natural numbering
    ISysFileInfoSort            m_ifisFileSort;

    // For loading icons
    QFileIconProvider           m_qfipIconProvider;
    QIcon                       m_qicnExeIcon;

    // References to UI elements so we can read settings and enable/disable actions
    IUIMenuBar* &               m_rpuimbMenuBar;
    IUIToolBar* &               m_rpuitbToolBar;
    IUIRename* &                m_rpuirRenameUI;

    // Application settings
    QSettings &                 m_rqsetSettings;

    // Tables for showing current name and preview of current rename options
    QTableWidget*               m_pqtwNameCurrent;
    QTableWidget*               m_pqtwNamePreview;

    // Stack of directory paths for back and forward buttons
    QStack<QString>             m_qsqstrBackStack;
    QStack<QString>             m_qsqstrForwardStack;

    // Stores the directory and file names for the last rename operation so it can be undone
    QString                     m_qstrUndoDirectory;
    QStringList                 m_qstrlUndoRenameFrom;
    QStringList                 m_qstrlUndoRenameTo;

    // Used for substituting invalid characters with alternatives
    IRenameInvalidCharSub       m_icsInvalidCharSub;

    // Indicates if the directory contents should be automatically refreshed when the contents of the directory changes or a file is modified
    bool                        m_bAutoRefresh;

    // Indicates if files should be opened when double clicked
    bool                        m_bOpenFileWhenDblClicked;

    // Indicates if hidden files are being shown for the directory
    bool                        m_bShowHiddenFiles;

    // Indicates if files short be re-sorted after a rename operation
    bool                        m_bReSortFileListAfterRename;

    // Height of each row in table, which can be adjusted to make the rows more or less tightly spaced
    bool                        m_bUseUserDefinedRowHeight;
    int                         m_iUserDefinedRowHeight;

    // Indicates if rows in the preview column should be highlighted when the name is different to the current name
    bool                        m_bNameChangeColourText;
    QColor                      m_qcolNameChangeTextColour;
    bool                        m_bNameChangeHighlightRow;
    QColor                      m_qcolNameChangeHighlightColour;

    // Stores default table font so it can be restored if the user disabled the custom font option
    QFont                       m_qfntDefaultFont;
    bool                        m_bUseAlternativeFont;

    // Used to avoid circular syncing between the two tables
    bool                        m_bSyncSelection;

    // Number of files that will be renamed with the current settings - for auto-numbering purposes
    int                         m_iNumFilesToRename;

    // Stores the string that represents "My Computer" on Windows (or "This PC" on windows 8.1, or "Computer" in Windows 10)
    QString                     m_qstrMyComputerPath;

    // Indicates if the currnt path being displayed in My Computer
    bool                        m_bDisplayingMyComputer;

    // Indicates which meta tags have been read for the currently open directory
    bool                        m_bMetaTagsReadMusic;
    bool                        m_bMetaTagsReadExif;

    // Indicates if all Exif tags should be read or just the basic subset
    bool                        m_bExifAdvancedMode;

    // Variables for right click context menu
    int                         m_iContextMenuRowClicked;

    QMenu*                      m_pqmenuEmptyAreaContextMenu;
    QActionGroup*               m_pqacgSortGroup;
    QAction*                    m_pqactSortByName;
    QAction*                    m_pqactSortByType;
    QAction*                    m_pqactSortByModified;

    QMenu*                      m_pqmenuFolderContextMenu;
    QAction*                    m_pqactRenameFolder;
    QAction*                    m_pqactOpenFolder;

    QMenu*                      m_pqmenuFileContextMenu;
    QAction*                    m_pqactRenameFile;
    QAction*                    m_pqactViewProperties;

    // Constants
    const int                   m_kiShowRenameProgressAfterMS = 1000;
    const int                   m_kiShowRenameProgressFileNum = 500;

public:
    // Constants for roles under which data is stored
    enum                        DataRoles {FlaggedForRenme = Qt::UserRole, FileInfo, MusicMeta, ExifMeta};

public:
    IUIFileList(IUIMainWindow* pmwMainWindow);

private:
    // Creates and initialieses QTableWidget object
    void ItitialiseTable(QTableWidget* & rpqtwTable, const QString & krqstrTitle);

public:
    // After creation of the window this is called to set initial directory and proccess command line parameters
    void InitAfterCreate();

private:
    // Checks command line parameters for start directory and if none calls function to set default start directory
    void InnitStartDir(const QStringList & krqstrlArguments);

    // Checks command line parameters for rename profile and if none calls function to set default rename settings
    void InnitRenameSettings(const QStringList & krqstrlArguments);

    // Configures rename settings based on preferences
    void SetInitialRenameSettings();

public:
    // Saves current rename settings and directory
    void SaveSessionSettings();

    // Should be passed AllEntries, Dirs, Files or Drives, and returns a list which includes/doesn't include hidden files based on current settings
    QFileInfoList GetDirectoryFileList(QDir::Filters m_qdirfFileFilter);

    // Called when opening directory via address bar, bookmarks or open directory dialog.  Opens directory and manages back/forward buttons
    void OpenDirectory(QString qstrPath);

private:
    // Restores directory from last session
    void RestoreLastSessionDirectory();

    // Set directory to display
    bool SetDirectory(QString qstrPath);

    // Populates tables with directory listing
    void PopulateTablesDirectory();   

    // Populates the table with a list of drives (My Computer)
    void PopulateTablesComputer();

    // Clears contents of both tables and cleares meta read flags
    void ClearTableContents();

    // Removes all files and directories from file system watcher
    void ClearFSWatcher();

    // Updates file info, icons and FS watcher after rename operation
    void RefreshDirectoryPostRename();

    // Hard refresh discards all data in the table (including meta data) and reloads everything
    void RefreshDirectoryHard();

    // Soft refresh compares the table contents to the current directory contents and adds/removes rows as necessary
    void RefreshDirectorySoft();
    void RemoveRows(const QList<int> & krqlstRemoveRows);
    void AddFiles(const QList<int> & krqlstAddIndexes, const QFileInfoList & krqfilFileList);
    void AddFile(const QFileInfo & krqfiNewFile, const int kiRow);

public:
    // Sets flags FlaggedForRenme role based on current rename settings
    void FlagItemsForRenaming();
    void FlagSelectedItemsForRenaming();
    void FlagItemsForRenamingByExtension();

    // If the rename operation requies Music/Exif meta data and there are no relevant meta data present in the file, this unflags the file for renaming
    void UnflagItemsForRenamingIfNoMeta();

    // Checks for meta tags and if present initiates read of the appropriate data, returning false if read of data is cancelled by user
    void ReadMetaTags();

    // These functions are responsible for reading the meta tags
    void ReadMetaTagsMusic(const bool kbForceReRead = false);
    void ReadFileMetaTagsMusic(QTableWidgetItem* pqtwiFileItem);
    void ReadMetaTagsExif(const bool kbForceReRead = false);
    void ReadFileMetaTagsExif(QTableWidgetItem* pqtwiFileItem);

    // Called if invalid character substitutions are changed in the preference menus as substitutions in tags must be redone
    void ReReadMetaTags();
    void ReReadMusicTags();
    void ReReadExifTags();

private:
    // Sets hidden file filder based on m_bShowHiddenFiles
    void SetHiddenFileFilter();

private slots:
    // Sets whether to show hidden file state and refreshes if necessary
    void SetHiddenFileState();

    // When the selected rows in one table chage this functions sync the row selection to the other table and updates the file names
    void SelectionChanged();
    void SyncSelectionXToY(QTableWidget* pqtwSyncFrom, QTableWidget* pqtwSyncTo);

    // When one table scroll position changes these functions sync the other table scroll position to match
    void SyncScrollPreviewToCurrent();
    void SyncScrollCurrentToPreview();

    // Displays file open dialog so the user can select a view to view properties
    void OpenFilePropertiesDialog();

    // Folder Navigation slots
    void OpenDirectoryDialog();
    void SetDirectoryFromAddressBar();
    void NavigateBack();
    void NavigateForward();
    void NavigateUp();
    void NavigateRefresh();
    void NavigateToStartDirectory(const bool kbStartupCall = false);
    void EnableBackForwardActions();

    // Navigates to the directory at the passed row, or does nothing if it's a file
    void OpenItemAtRow(const int kiRow);
    void OpenDirectoryContext()             {OpenItemAtRow(m_iContextMenuRowClicked);}

    // Opens a file properties dialog for the file at the specified row
    void OpenFilePropertiesForRow(const int kiRow);
    void OpenFilePropertiesContext()        {OpenFilePropertiesForRow(m_iContextMenuRowClicked);}

    // Opens a dialog for renaming the file at the specified row
    void OpenRenameFileDlgForRow(const int kiRow);
    void OpenRenameFileDlgContext()         {OpenRenameFileDlgForRow(m_iContextMenuRowClicked);}
    void OpenRenameFileDlgForCurrentRow();

    // Displays context menu when file in preview pane is right clicked
    void ShowContextMenu(QPoint qpntClickPoint);

    // Creates right click context menus and associated actions
    void CreateContextMenus();

    // Sets table to be sorted by specified attribute
    void SortByName();
    void SortByType();
    void SortByModified();

    // Resorts the table when the sort attribute changes
    void ResortTable(const int kiFolderSortOrder, const int kFileSortOrder);
    void ResortTableFilesOnly(const int kFileSortOrder) {ResortTable(INT_MAX, kFileSortOrder);}
    void ResortRows(const int kiStart, const int kiEnd, const int kiSortOrder);

    // Returns the number of folders in the current directory listing
    int GetNumFolders();

    // Called when QFileSystemWatcher detects a change in the current directory
    void DirectoryChanged(const QString & krqstrDirectory);
    void FileChanged(const QString & krqstrFile);

    // Prints watch list for debugging purposes
    void PrintWatchList();

    // Moves selected rows up and down for manual re-ordering by user
    void MoveSelectionUp();
    void MoveSelectionDown();

private:
    // Functions for moving individual selection blocks from the selection list up and down
    int MoveSelectionUp(QTableWidget * pqtwTable, const int kiTop, const int kiSelectionTop, const int kiSelectionBottom);
    int MoveSelectionDown(QTableWidget * pqtwTable,  const int kiBottom, const int kiSelectionTop, const int kiSelectionBottom);

public:
    // Updates the preivew name only or name and extension
    void GeneratePreviewName();
    void GeneratePreviewNameAndExtension();

    // Refreshes flags indicating which files should be renamed and regenerates previews
    void RenameElementsSettingsChanged();

private:
    // Compares the current name to the preivew name and highlights the preivew row if it's different
    void HighlightRowsWithModifiedNames(const bool kbForceRedraw = false);

public:
    // Renames files to name shown in preview table
    void PerformRename();

    // Reverts changes made by last rename operation
    void UndoRename();

private:
    // Called by PerformRename() and UndoRename() to rename files.  pqlstiRows is only passed by PerformRename() so can be used to determine if we're renaming or undoing
    void RenameFiles(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, QList<int>* pqlstiRows = nullptr);

    // Renames files by first giving them an intermediate name and then renaming to the final name to avoid naming conflicts
    void RenameFilesIntermediate(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, QList<int>* pqlstiRows = nullptr);

    // Combines above two functions into one, so this can rename forward, backward or intermediate.  I decided I prefer them separate so this isnt used.
    void RenameFilesValidated(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, QList<int>* pqlstiRows = nullptr);

    // Strips off invalid characters from the end of the passed filename
    void RemoveInvalidTrailingCharacters(QString & rqstrFileName);

    // Returns true if there will be no conflicting names in the end result of a rename operation
    bool RenameEndResultValid();

    // Returns true if there will be no conflicting at any point during the rename process
    bool RenameProccessValid(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, const bool kbValidateBackwards);
    bool RenameProccessValidForward(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName)   {return RenameProccessValid(rqstrlCurrentName, rqstrlNewName, false);}
    bool RenameProccessValidBackward(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName) {return RenameProccessValid(rqstrlCurrentName, rqstrlNewName, true);}

    // QDir:rename() doesn't give a reason why the rename failed so this function tries to determine the reason
    QString DetermineReasonForFailure(const QString & krqstrCurrentName, const QString & krqstrFailedRename, const QFileInfo & krqfiFileInfo);

    // Displays dialog box allowing the user to rename the currently highlighted file
    void RenameHighlightedFile();

public:
    // Accessors for flags
    bool AutoRefreshEnabled()               {return m_bAutoRefresh;}
    void SetAutoRefreshEnabled(const bool kbAutoRefresh);
    bool SaveSortOrder()                    {return m_ifisFileSort.GetSaveSortOrder();}
    void SetSaveSortOrder(const bool kbSaveSortOrder)                                   {m_ifisFileSort.SetSaveSortOrder(kbSaveSortOrder);}
    bool OpenFileWhenDblClicked()           {return m_bOpenFileWhenDblClicked;}
    void SetOpenFileWhenDblClicked(const bool kbOpenFileWhenDblClicked)                 {m_bOpenFileWhenDblClicked = kbOpenFileWhenDblClicked;}
    bool ReSortFileListAfterRename()        {return m_bReSortFileListAfterRename;}
    void SetReSortFilesAfterRename(const bool kbReSortFileListAfterRename)              {m_bReSortFileListAfterRename = kbReSortFileListAfterRename;}

    // Get and set row height
    int GetCurrentRowHeight();
    bool GetUseUserDefinedRowHeight()       {return m_bUseUserDefinedRowHeight;}
    int GetUserDefinedRowHeight()           {return m_iUserDefinedRowHeight;}
    void SetUserDefinedRowHeight(const bool kbUseUserDefinedRowHeight, const int kiRowHeight);
    void SetUseAlternativeFont(const bool kbUseAlternativeFont, const QFont & krqfntSelectedFont);
    void SetRowHeightAndIconSize();

    // Functions for getting and setting the highlight colours
    bool GetNameChangeColourText()          {return m_bNameChangeColourText;}
    QColor & GetNameChangeTextColour()      {return m_qcolNameChangeTextColour;}
    bool GetNameChangeHighlightRow()        {return m_bNameChangeHighlightRow;}
    QColor & GetNameChangeHighlightColour() {return m_qcolNameChangeHighlightColour;}
    void SetHighlightSettings(const bool kbNameChangeColourText, const QColor & krqcolNameChangeTextColour, const bool kbNameChangeHighlightRow, const QColor & krqcolNameChangeHighlightColour);

    // Functions to get and set font related variables
    bool GetUseAlternativeFont()            {return m_bUseAlternativeFont;}
    void RestoreDefaultFonts();
    const QFont & GetCurrentFont();
    void SetTableFont(const QFont & krqfntNewFont);

    // Indicates if meta tags have been read
    bool MetaTagsReadMusic()                {return m_bMetaTagsReadMusic;}
    bool MetaTagsReadExif()                 {return m_bMetaTagsReadExif;}
    bool ExifAdvancedModeEnabled()          {return m_bExifAdvancedMode;}

    // Sets Exif to advaced/basic mode causing Exif menu to be recreated.  Returns true if Exif tags need re-reading
    bool SetExifAdvancedMode(const bool kbExifAdvancedMode);

    // Other Accessors
    QString CurrentPath()                   {return m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path();}
    QString CurrentDirectory()              {return m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.dirName();}
    IRenameInvalidCharSub & GetInvCharSub() {return m_icsInvalidCharSub;}

protected:
    // For handling shortcut keys
    void keyPressEvent(QKeyEvent* pqkeEvent);
};


class CurrentTableHighlightDelegate : public QStyledItemDelegate
{
private:
    // Pointer to file list for getting directory listings
    IUIFileList*        m_puifmFileList;

public:
    CurrentTableHighlightDelegate(IUIFileList* puifmFileList)  : QStyledItemDelegate(puifmFileList) {m_puifmFileList = puifmFileList;}

    // Adjusts background and higlight colour when rows are selected
    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};


class PreviewTableHighlightDelegate : public QStyledItemDelegate
{
private:
    // Pointer to file list for getting directory listings
    IUIFileList*        m_puifmFileList;

public:
    PreviewTableHighlightDelegate(IUIFileList* puifmFileList)  : QStyledItemDelegate(puifmFileList) {m_puifmFileList = puifmFileList;}

    // Adjusts background and higlight colour when rows are selected
    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};


#endif // IUIFileList_h
