Last but not least , we explore FILE SYSTEMS ( inode, inotify api )

Basically we have two folders SOURCE and BACKUPS.

We create a tree of "file/directory-names", with each one connected to its appropriate i-node.

These i-nodes are related to the files themselves, so we keep them in a List (these i-nodes are part of the kernel so weattach the necessary information to one of our structs )

We try to simulate the file system operations and these 4 steps:

1) Create a Name-Tree and a List of I-nodes for both SOURCE and BACKUP ( if backup doesn't exist , copy source and skip step2)
2) Synchronise the Name-Trees , list of Inodes of the two.
3) Use inotfy API ( informs us for any changes in our folders). Monitor SOURCE for any changes and act as instructed, by "copying" the changes to backup(keeping them synchronised).