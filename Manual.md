# FORmicidae Tracker Tag Scanner Manual

The application is fearly simple. It consist of two panel:

 * Scan: a panel that display the video live feed, and some camera an detection parameter
 * Data: the log of the data

## Selecting a camera

You must first select the camera you want to use. Alternatively you
can simply open an image file. This is done in the `Devices` menu.

If no camera are listed and you have installed `fort-tag-scanner` with
snap, please verify that you did authorize `fort-tag-scanner` to
access camera by using the following command (and -1 kudos for not
reading the installation instructions completely).

``` bash
sudo snap connect fort-tag-scanner:camera :camera
```

## Detecting tags

Automatically the camera live feed will be displayed. You must now
select the tag family you want to use and some other detection
settings:
 * Min BW threshold: the threshold required to detect a black/white
   border, similar to the `leto` `min-bw-diff-threshold`.
 * Min Cluser Size: Minimal number of pixel that could be considered a quad/tag.
 * Save after N match: if a tag would consecutively be detected this
   number of time, the video feed will be stopped and the tag value
   will be added to the `Data` panel.

Once a tag is detected, the video feed will be stopped, a sound alert
will be trigerred. Youcan re-arm the live feed by presisng
`<spacebar>`.


## Editing Data

The `Data` panel is a very crude spreadsheet with the following column:

* Datetime: the date of the detection
* TagID: the TagID detected.
* AntID: optionally, see below.
* Comment: Some comment the user can type in.

## Matching TagID and AntID

If you have already matched tag with ants in FORT Studio in a nice
myrmidon file, you can load this myrmidon file unsing the `Load
Myrmidon File` command. Once done, every TagID will be matched to the
corresponding AntID if it exist, or left blank.

Please note that `fort-tag-scanner` open the myrmidon file in
data-less mode. Normally opening a myrmidon file would require you to
have the tracking data on the same computer. In data-less mode, no
tracking data is opened, and only identification/metadata are
read. Therefore if the tracking data lives in another computer, you
can only copy the `*.myrmidon` file to the computer where the scan are
performed.

## Saving to a CSV

Simply use the `Save data as CSV` function.
