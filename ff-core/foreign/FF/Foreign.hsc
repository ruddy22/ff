{-# OPTIONS_GHC -Wno-orphans #-}

{-# LANGUAGE ForeignFunctionInterface #-}
{-# LANGUAGE RecordWildCards #-}

module FF.Foreign () where

import           Data.ByteString (packCString)
import           Data.Foldable (for_)
import           Data.Text.Encoding (decodeUtf8)
import           Foreign (Ptr, StablePtr,
                          Storable (alignment, peek, peekByteOff, poke,
                                    pokeElemOff, sizeOf),
                          deRefStablePtr, mallocBytes)
import           Foreign.C (peekCAString)

import qualified FF
import           FF.Storage (StorageConfig)
import qualified FF.Storage as FF
import           FF.Types (NoteView (NoteView, nid, status, text), Status)

#def typedef char * NoteIdZ;

#def typedef char * Utf8StringZ;

#def typedef enum Status {Active, Archived, Deleted} Status;

#def typedef struct NoteView {
    NoteIdZ     nid;
    Status      status;
    Utf8StringZ text;
} NoteView;

instance Storable NoteView where
    sizeOf    _ = #size      NoteView
    alignment _ = #alignment NoteView
    peek p = do
        nid    <- fmap FF.DocId . peekCAString =<< #{peek NoteView, nid} p
        status <- toEnum <$> #{peek NoteView, status} p
        text   <- fmap decodeUtf8 . packCString =<< #{peek NoteView, text} p
        pure NoteView{..}

foreign export ccall loadActiveNotes
    :: StablePtr StorageConfig -> IO (Ptr NoteView)
loadActiveNotes :: StablePtr StorageConfig -> IO (Ptr NoteView)
loadActiveNotes ptrStorage = do
    storage <- deRefStablePtr ptrStorage
    notes <- FF.runStorage storage FF.loadActiveNotes
    notesC <- mallocBytes $ length notes * sizeOf (undefined :: NoteView)
    for_ (zip [0..] notes) $ \(i, note) -> pokeElemOff notesC i note
    pure notesC
