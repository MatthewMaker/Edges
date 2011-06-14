#!/bin/csh
#\
exec ./silhouette -f $0 $*

wm withdraw .
set main .main
toplevel $main
wm title $main "Silhouette Edge Detection"

wm minsize $main 400 300
wm maxsize $main 850 850

# Position the user interface approximately in the centre of the screen.
set xPos [expr [winfo screenwidth $main]/3 - [winfo reqwidth .]]
set yPos [expr [winfo screenheight $main]/5 - [winfo reqheight .]]
wm geometry $main +$xPos+$yPos

# Width and height of the screen for showing image.
set screenWidth 850
set screenHeight 850

# For calling initial setup once
set setup 0

set typelist {
    {"Wavefront OBJ Format"	{".obj"} }
    {"Stanford Model Format"	{".smf"} }
    {"Univ. Washington Models"	{".m"}   }
    {"WeirdAss MSH"		{".msh"}   }
}
set initdir "../../data"
set ext ".iv"

set hidden 1
set sil 0
set strip 1
set opt 2

#------------------------------------------------------------------------------
# Constants:
#------------------------------------------------------------------------------

# translation factor
set MOVE 0.005

# trackball scale facotr
set TRACK_SCALE 35

#------------------------------------------------------------------------------
# Variables:
# b1   - mouse button 1 depressed
# b2   - mouse button 2 depressed
# b3   - mouse button 3 depressed
# mx   - mouse x position
# my   - mouse y position
# px   - mouse x position for click
# py   - mouse y position for click
# vtx  - view translate x
# vtx  - view translate x
# vty  - view translate y
# vtz  - view translate z
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------

# ------------- #    
# The menu bar. #
# ------------- #
set l $main.mbar
frame $l
# -relief raised -bd 4
pack $l -side top -anchor n -fill x -padx 2m

# ---------- #
# File Menu  #
# ---------- #
button $l.load -text "Load 3D Model" -command Loadfile
button $l.reset -text "Reset" -command ResetAll

checkbutton $l.wire -text "Hidden Line" -variable hidden -command { DP }
checkbutton $l.sil -text "Silhouette Edges" -variable sil -command { DP }
checkbutton $l.strip -text "Draw Triangle Strips" -variable strip -command { DP }

frame $l.option -relief groove -bd 4
set lo $l.option
radiobutton $lo.opt0 -text "None" -variable opt -value 0 -command { DP }
radiobutton $lo.opt1 -text "PMRTree" -variable opt -value 1 -command { DP }
radiobutton $lo.opt2 -text "POTree" -variable opt -value 2 -command { DP }
 
button $l.quit -text "Quit" -command "exit"

pack $l.load $l.reset -side left
pack $l.quit -side right

pack $lo.opt0 $lo.opt1 $lo.opt2 -side left
pack $l.wire $l.sil $l.strip $lo -side left -pady 1m
#pack $l -side bottom -anchor s -fill x -padx 2m

# Draw Param --------------------------------------------------#
proc DP {} {
    global hidden sil strip opt
    gr_drawparam $hidden $sil $strip $opt
    Render
}

proc Hidden {} {
    global hidden
    if { $hidden == 0 } {
	set hidden 1
    } else {
	set hidden 0
    }
    DP
}

proc Sil {} {
    global sil
    if { $sil == 0 } {
	set sil 1
    } else {
	set sil 0
    }
    DP
}

# ------------------------------------------------------------ #
# Load and Save                                                #
# ------------------------------------------------------------ #
proc Loadfile {} {
    global typelist initdir

    set filename [tk_getOpenFile -filetypes $typelist -initialdir $initdir]
    if { $filename != "" } {
	gr_load $filename
    }
    ResetAll
    Render
}

# ------------------------------------------------------------ #
# MakeOGLRaster                                                #
# ------------------------------------------------------------ #
#proc MakeOGLRaster {width height} {
#    global oglRaster main oglFrame
#
#    set oglFrame $main.oglFrame
#    frame $oglFrame -bd 2 -relief sunken
#    set oglRaster $oglFrame.ogl 
#    ogl_raster $oglRaster {
#	GLX_RGBA
#	GLX_RED_SIZE   2
#	GLX_BLUE_SIZE  2
#	GLX_GREEN_SIZE 2
#	GLX_DEPTH_SIZE 12
#        GLX_DOUBLEBUFFER
#    } -width $width -height $height;
#    
#    pack $oglRaster -expand yes 
#    pack $oglFrame -expand yes -padx 2m -pady 2m
#}
#
# ------------------------------------------------------------ #
# Frame for a OGL raster.                                      #
# ------------------------------------------------------------ #
#
#MakeOGLRaster $screenWidth $screenHeight

set oglFrame $main.oglFrame
frame $oglFrame -bd 2 -relief sunken
set oglRaster $oglFrame.ogl 
togl $oglRaster \
        -rgba true \
        -redsize 4 \
        -greensize 4 \
        -bluesize 4 \
        -depth true \
        -depthsize 16 \
        -double true \
        -width $screenWidth \
        -height $screenHeight
#pack $raster -padx 2m -pady 2m -fill both -expand yes 
pack $oglRaster -padx 2m -pady 2m -fill both -expand yes 
pack $oglFrame -expand yes -padx 2m -pady 2m


# -------- #
# Bindings #
# -------- #

# ----- short cut keys ------------
bind all <Q> exit
bind all <q> exit 
bind all <P> ResetPosition
bind all <p> ResetPosition 
bind all <O> ResetOrientation
bind all <o> ResetOrientation
bind all <A> ResetAll
bind all <a> ResetAll
bind all <l> Loadfile
bind all <L> Loadfile
bind all <h> Hidden
bind all <s> Sil

# GUI initialization
bind $main <Configure> {
    global oglRaster screenWidth screenHeight setup mx my b1 b2 b3

    if {$setup == 0} {
	InitialSetup
	set setup 1

        set b1 0
        set b2 0
        set b3 0

        set mx %x
        set my %y
    }
    
    set screenWidth [lindex [$oglRaster configure -width] 4]
    set screenHeight [lindex [$oglRaster configure -height] 4]
    
    Render
}

# repaint when frame exposed
bind $oglFrame <Expose> {
    Render
}

# repaint when canvas exposed
bind $oglRaster <Expose> {
    Render
}

#------------------------------------------------------------------------------
# mouse events handlers
#------------------------------------------------------------------------------
# set params when 1st mouse button is pressed
bind $oglRaster <Button-1> {
    global b1 mx my px py
    global mode MODE_JO 

    set b1 1
    set mx %x
    set my %y
    set px %x
    set py %y
}

# bind $oglRaster <Control-Button-1> {
#     global b1 mx my px py
#     global mode MODE_JO 
# 
#     set b1 1
#     set mx %x
#     set my %y
#     set px %x
#     set py %y
# }

# set params when 1st mouse button is released
bind $oglRaster <ButtonRelease-1> {
    global mx my px py

    set mx %x
    set my %y
    set b1 0
}

# bind $oglRaster <Control-ButtonRelease-1> {
#     global b1 mx my px py
#     global picked M_SELECT M_DEFAULT
# 
#     gr_pick : "$px $py"
#     Render
# 
#     set mx %x
#     set my %y
#     set b1 0
# }

# set params when 2nd mouse button is pressed
bind $oglRaster <Button-2> {
    global b2 mx my
    set b2 1
    set mx %x
    set my %y
}

# set params when 2nd mouse button is released
bind $oglRaster <ButtonRelease-2> {
    global b2 mx my
    set mx %x
    set my %y
    set b2 0
}

# set params when 3nd mouse button is pressed
bind $oglRaster <Button-3> {
    global b3 mx my
    set b3 1
    set mx %x
    set my %y
}

# set params when 3nd mouse button is released
bind $oglRaster <ButtonRelease-3> {
    global b3 mx my
    set mx %x
    set my %y
    set b3 0
}

# handle motion events
bind $oglRaster <Motion> {
    global b1 b2 b3 mx my vtx vty vtz
    global mode MODE_PO MODE_JO MOVE TRACK_SCALE

    if {$b1!=0 || $b2 != 0 || $b3 != 0} {
       
    # handle orientation stuff
       set xdiff [expr %x-$mx]
       set ydiff [expr $my-%y]

       if {$b1 == 1} {
         gr_trackball : "$mx $my" "%x %y" $TRACK_SCALE
       }

       if {$b2 == 1} {
         set vtx [expr $vtx + $xdiff * $MOVE]
         set vty [expr $vty + $ydiff * $MOVE]
       }

       if {$b3 == 1} {
         set vtz [expr $vtz - $ydiff * $MOVE * 3]
       }

       gr_translate : "$vtx $vty $vtz"
       Render

    # save last mouse position
    set mx %x
    set my %y
	}
}

# ------------ #
# InitialSetup #
# ------------ #
proc InitialSetup {} {
#   gr_initialize
    ResetPosition
}

#------------------------------------------------------------------------------
# ResetAll 
#------------------------------------------------------------------------------
proc ResetAll {} {

    global vtx vty vtz

    # reset position
    set vtx 0.0
    set vty 0.0
    set vtz 0.0
    gr_translate : "$vtx $vty $vtz"

    # reset orientation
    gr_identity :
    Render
}

#------------------------------------------------------------------------------
# ResetPosition
#------------------------------------------------------------------------------
proc ResetPosition {} {
    global vtx vty vtz

    # translate view back
    set vtx 0.0
    set vty 0.0
    set vtz 0.0
    gr_translate : "$vtx $vty $vtz"

    Render
}

#------------------------------------------------------------------------------
# ResetOrientation
#------------------------------------------------------------------------------
proc ResetOrientation {} {
   gr_identity :
   Render
}

#------------------------------------------------------------------------------
# Render
#------------------------------------------------------------------------------
proc Render {} {
    global oglRaster screenWidth screenHeight buffer hidden sil 

    $oglRaster render
}

