#include <QApplication>

int main(int argc, char const *argv[]) {
//     Config { dataDir = Just dataDir } <- loadConfig
//     timeVar                           <- newTVarIO =<< getRealLocalTime
//     let storage = Storage dataDir timeVar
    QApplication app(argc, argv);
//         setOrganizationDomain app "ff.cblp.su"
//         setOrganizationName   app "ff"
//         setApplicationName    app "ff"
//         setApplicationVersion app $ showVersion version
//         mainWindow <- newMainWindow storage
//         QWidget.show mainWindow
    app.exec();
    return 0;
}

// withApp :: (QApplication -> IO a) -> IO a
// withApp = withScopedPtr $ getArgs >>= QApplication.new

// newMainWindow :: Storage -> IO QMainWindow
// newMainWindow storage = do
//     this <- QMainWindow.new
//     setCentralWidget this =<< do
//         tabs <- QTabWidget.new
//         addTab_ tabs "Agenda" =<< newAgendaWidget storage
//         pure tabs
//     setWindowTitle this "ff"

//     -- https://wiki.qt.io/Saving_Window_Size_State
//     void $ onEvent this $ \(_ :: QShowEvent) -> do
//         withScopedPtr QSettings.new $ \settings -> do
//             void
//                 $   value settings "mainWindowGeometry"
//                 >>= toByteArray
//                 >>= restoreGeometry this
//             void
//                 $   value settings "mainWindowState"
//                 >>= toByteArray
//                 >>= restoreState this
//         pure False
//     void $ onEvent this $ \(_ :: QCloseEvent) -> do
//         withScopedPtr QSettings.new $ \settings -> do
//             setValue settings "mainWindowGeometry"
//                 =<< QVariant.newWithByteArray
//                 =<< saveGeometry this
//             setValue settings "mainWindowState"
//                 =<< QVariant.newWithByteArray
//                 =<< saveState this
//         pure False

//     pure this

// addTab_ :: QWidgetPtr widget => QTabWidget -> String -> widget -> IO ()
// addTab_ tabs name widget = void $ addTab tabs widget name

// sectionLabels :: ModeMap String
// sectionLabels = FullMap $ \case
//     Overdue  -> "Overdue"
//     EndToday -> "Due today"
//     EndSoon  -> "Due soon"
//     Actual   -> "Actual"
//     Starting -> "Starting soon"

// sectionIndices :: ModeMap Int
// sectionIndices = FullMap fromEnum

// sectionLabel :: TaskMode -> Int -> String
// sectionLabel mode n =
//     concat [FullMap.lookup mode sectionLabels, " (", show n, ")"]

// newAgendaWidget :: Storage -> IO QToolBox
// newAgendaWidget (Storage dataDir timeVar) = do
//     this <- QToolBox.new
//     void $ onEvent this $ \(_ :: QShowEvent) ->
//         -- TODO set the first item as current
//                                                 pure False
//     modeSections <- sequence . FullMap $ newSection this
//     runStorage dataDir timeVar loadActiveNotes
//         >>= traverse_ (addNote this modeSections)
//     pure this

// newSection :: QToolBoxPtr toolbox => toolbox -> TaskMode -> IO QVBoxLayout
// newSection this section = do
//     widget <- QWidget.new
//     _      <- addItem this widget label
//     box    <- QVBoxLayout.newWithParent widget
//     addStretch box
//     pure box
//     where label = sectionLabel section 0

// addNote
//     :: QBoxLayoutPtr layout => QToolBox -> ModeMap layout -> NoteView -> IO ()
// addNote this modeSections note = do
//     today <- getUtcToday
//     item  <- newNoteWidget note
//     let mode       = taskMode today note
//     let sectionBox = FullMap.lookup mode modeSections
//     n <- sectionSize sectionBox
//     insertWidget sectionBox n item
//     setItemText this
//                 (FullMap.lookup mode sectionIndices)
//                 (sectionLabel mode $ n + 1)

// newDateWidget :: String -> Day -> IO QHBoxLayout
// newDateWidget label date = do
//     box <- QHBoxLayout.new
//     addWidget box =<< QLabel.newWithText label
//     addWidget box =<< do
//         dateEdit <-
//             QDateEdit.newWithDate
//                 =<< QDate.newWithYearMonthDay (fromInteger y) m d
//         setCalendarPopup dateEdit True
//         setReadOnly      dateEdit True
//         pure dateEdit
//     pure box
//     where (y, m, d) = toGregorian date

// newNoteWidget :: NoteView -> IO QFrame
// newNoteWidget NoteView { text, start, end } = do
//     this <- QFrame.new
//     setFrameShape this StyledPanel
//     do
//         box <- QVBoxLayout.newWithParent this
//         addWidget box =<< QLabel.newWithText (Text.unpack text)
//         addLayout box =<< do
//             fieldsBox <- QHBoxLayout.new
//             addLayout fieldsBox =<< newDateWidget "Start:" start
//             whenJust end $ addLayout fieldsBox <=< newDateWidget "Deadline:"
//             addStretch fieldsBox
//             pure fieldsBox
//     pure this

// -- Because last item is always a stretch.
// sectionSize :: QLayoutConstPtr layout => layout -> IO Int
// sectionSize layout = pred <$> count layout
