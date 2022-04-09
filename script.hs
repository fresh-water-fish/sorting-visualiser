-- Imports
-- import Data.Aeson.Encoding (pairs)
import Data.Char (intToDigit, digitToInt)

{-
  -   / / / / / / / / / / /
  -  COMPRESSION FUNCTIONS 
  - / / / / / / / / / / /
-}
{-
  -  select a run of repeated characters from the start of a string,
  -  with the run being as long as possible
-}
chew :: String -> String
chew xs = takeWhile (== head xs) xs

{-
  -  select a run of repeated characters from the start of the string, 
  -  with the run comprising of at most nine characters
-}
nibble :: String -> String
nibble xs = take 9 (chew xs)

{-
  -  split a string into a list of runs of repeated characters,
  -  with each run comprising of at most nine characters
-}
runs :: String -> [String]
-- runs xs = [ys | let ys = nibble xs, not (null ys)]
runs [] = []
runs xs = nibble ys:runs (drop (length ys) xs)
        where ys = nibble xs

{-
  -  transorm a string into a list of pairs,
  -  comprising the character from each run together,
  -  with its number of repititions
-}
encode :: String -> [(Char,Int)]
encode xs = [(head x,length x) | x <- runs xs]

{-
  -  flatten a list of pairs of characters and digits to a string
-}
flatten :: [(Char,Int)] -> String
flatten [] = []
flatten xs = fst x:intToDigit (snd x):flatten (tail xs)
           where x = head xs

{-
  -  compress a string using run-length encoding
-}
compress :: String -> String
compress xs = flatten (encode xs)

{-
  -   / / / / / / / / / / / /
  -  DECOMPRESSION FUNCTIONS 
  - / / / / / / / / / / / /
-}
{-
  -  perform the inverse function to encode
-}
decode :: [(Char,Int)] -> String
decode = foldr (\x -> (++) (replicate (snd x) (fst x))) []

{-
  -  perform the inverse function to flatten
-}
expand :: String -> [(Char,Int)]
expand xs = [(xs !! n,digitToInt (xs !! (n+1))) | n <- filter even [0..length xs - 1]]

{-
  -  perform the inverse function to compress
-}
decompress :: String -> String
decompress xs = decode (expand xs)
