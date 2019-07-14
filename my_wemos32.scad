// Gehäuse für Wemos Lolin 32
// (c) FP, Mai 2019

w32_laenge= 65;
w32_breite= 27.6;
w32_tiefe= 12;
w32_wand= 1;

// Gehäuse

module gehaeuse() {
difference() {
  
  translate([-w32_wand,-w32_wand,-w32_wand])
    cube([w32_laenge+2*w32_wand,w32_breite+2*w32_wand,w32_tiefe+w32_wand]);
    
    // inneres
    cube([w32_laenge,w32_breite,w32_tiefe]);


    // Fenster ESP32

    esp32_xoff= 6.9; 
    esp32_yoff= 5.4;
    esp32_x= 17.8;
    esp32_y= 16.4;
    esp32_z= 5; // egal

   translate([esp32_xoff, esp32_yoff,-w32_wand])
        cube([esp32_x, esp32_y, esp32_z]);

    // Fenster OLED
    
    oled_xoff= 31.5;
    oled_yoff= 6.5;
    oled_x= 25;
    oled_y= 13.9;
    oled_z= 5; // egal
    
    translate([oled_xoff, oled_yoff,-w32_wand])
        cube([oled_x, oled_y, oled_z]);
    
    // USB Loch
    
    usb_xoff= w32_laenge-2;
    usb_yoff= 5.2;
    usb_x= 5; // egal
    usb_y= 9.6;
    usb_z= 4.6;

    translate([usb_xoff, usb_yoff,usb_z])
        cube([usb_x, usb_y, usb_z]);

    // Kabeldurchführung beim Radmesser, 
    // relativ zu USB, etwas manuell...

    #translate([usb_xoff, usb_yoff+16,usb_z+2])
        cube([usb_x, 5, 2]);
   
}

// Haltestifte (hier: Blöcke)

soff_x= 4;
soff_y= 4;
soff_z= 3.5;

translate([0,0,0]) cube([soff_x, soff_y, soff_z]);
translate([w32_laenge-soff_x,0,0]) cube([soff_x, soff_y, soff_z]);
translate([w32_laenge-soff_x,w32_breite-soff_y,0]) cube([soff_x, soff_y, soff_z]);
translate([0,w32_breite-soff_y,0]) cube([soff_x, soff_y, soff_z]);
}

module deckel() {
// deckel - Grundplatte

  translate([-w32_wand,-w32_wand,-w32_wand])
    cube([w32_laenge+2*w32_wand,w32_breite+2*w32_wand,w32_wand]);

  // Innenwand - Innenblock

  difference() {
    cube([w32_laenge,w32_breite,4]);
      translate([w32_wand, w32_wand, w32_wand])
    cube([w32_laenge-2*w32_wand,w32_breite-2*w32_wand,4]);
  }
}
  
// für das gewünschte Bauteil die Kommentare entfernen:

// gehaeuse();
// deckel();