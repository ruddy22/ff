module Main (main) where

import           Development.Shake (shakeArgs, shakeOptions, want)

main :: IO ()
main = shakeArgs shakeOptions $ want ["ff-qt"]
