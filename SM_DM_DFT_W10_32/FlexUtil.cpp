// FlexUtil.cpp : VSFlexGrid Pro 7.0 utilities
//

#include "stdafx.h"
#include "FlexUtil.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Utility functions to convert between Twips and screen Pixels
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
short lpx = 0;
short lpy = 0;

// called once to get constants
void GetDeviceConstants(void)
{
	HDC hdc = GetDC(NULL);
	lpx = GetDeviceCaps(hdc, LOGPIXELSX); 
	lpy = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);
}       

long XTwipsToPixels(long twips)
{
	if (!lpx) GetDeviceConstants();
	return MulDiv(lpx, (int)twips, 1440);
}

long YTwipsToPixels(long twips)
{
	if (!lpy) GetDeviceConstants();
	return MulDiv(lpy, (int)twips, 1440);
}  

long XPixelsToTwips(long pixels)
{
	if (!lpx) GetDeviceConstants();
	return MulDiv(pixels, 1440, lpx);
}

long YPixelsToTwips(long pixels)
{
	if (!lpx) GetDeviceConstants();
 	return MulDiv(pixels, 1440, lpy);
}  


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Utility functions to generate HTML
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BOOL IsNumber(LPCSTR str)
{
    BOOL hasDigit = FALSE;
    BOOL hasDecimal = FALSE;

    // empty strings are not numbers
    if (!str ||!*str) return FALSE;

    // scan the string
    for (LPCSTR ptr = str; ptr && *ptr; ptr = CharNext(ptr)) {
        
        switch (*ptr) {

            // plus, minus, and currency are OK before any numbers
            case '+':
            case '-':
            case '$':
                if (hasDigit) return FALSE;
                break;

            // commas are always OK
			case ',':
                break;

			// only one decimal point is allowed
            case '.':
                if (hasDecimal) return FALSE;
                hasDecimal = TRUE;
                break;
                
			// digits are OK anywhere
            default:
                if (*ptr >= '0' && *ptr <= '9') {
                    hasDigit = TRUE;
                    break;
                }
                return FALSE;
        }
    }
    
    // if you got here, you're a number
    return TRUE;
}

CString HTMLColor(long clr)
{
    // convert system colors into real color
    if (clr & 0x80000000)
        clr = GetSysColor((int)(clr & 0xFFFF));
    clr = (clr & 0xFFFFFF);

    // break color into RGB components
    int r = (clr & 0xff);
    int g = ((clr >>  8) & 0xff);
    int b = ((clr >> 16) & 0xff);

    // format into string
    CString strClr;
    strClr.Format("\"#%02x%02x%02x\"", r, g, b);
    return strClr; 
}

CString HTMLText(LPCSTR strRaw)
{
    CString strNew;

    // convert string
    for (LPCSTR ptr = strRaw; ptr && *ptr; ptr = CharNext(ptr)) {
        switch (*ptr) {
            case '&':
                strNew = strNew + "&amp;";
                break;
            case '<':
                strNew = strNew + "&lt;";
                break;
            case '>':
                strNew = strNew + "&gt;";
                break;
            case '\n':
                strNew = strNew + "<BR>\n";
                break;
            default:
                strNew = strNew + (*ptr);
        }
    }

    // to avoid funny rendering, make sure we don't return an empty string
    if (strNew.GetLength() == 0) strNew = "&nbsp;";
    
    // done
    return strNew;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// SaveGridToHTML(CVSFlexGrid* ctl, LPSTR fname) 
//
// saves the given FlexGrid control into an HTML file.
// fn is the file name for the HTML file, including path and extension.
// returns True if successful, False otherwise.
//
// we don't do pictures
// we don't do font sizing
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#if 1
BOOL SaveGridToHTML(CListCtrl* ctlList, LPSTR fname)
{

    char* sHeader2[14] = { "NO","STEP", "RESULT","T/T", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };
    int  nWidth[14] = { 40, 100, 90, 40, 70, 90, 75, 75, 65, 200, 200, 76, 500,10 }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};

    // create a dummy variant we can use when we want to omit optional parameters
    VARIANT vDefVal;
    V_VT(&vDefVal) = VT_ERROR;

    // variant to use as scratch
    COleVariant vCell;

    // additional width for HTML columns
    double EXTRAWIDTH = 1.3;
    
    // open the file, quit on failure
    FILE* f = fopen(fname, "wt");
    if (!f) return FALSE;
    
    // save page header
    CString strHdr;
    strHdr = "<HTML>\n<HEAD>\n"
             "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html;charset=windows-1252\">\n"
             "<META NAME=\"Generator\" CONTENT=\"VSFlexGrid Pro\">\n"
             "<TITLE>VSFlexGrid Pro</TITLE>\n"
             "</HEAD>\n<BODY>\n\n";
    fprintf(f, (LPCSTR)strHdr);
              
    // get total table width in pixels
    double tblwid = 0;
   
    for (int i = 0; i < MAX_COLUMN_NO; i++)
        tblwid += nWidth[i];// ctlList->GetColumnWidth(i); //ctl->GetColWidth(i);
   //// if (tblwid < ctlList->GetClientWidth()) tblwid = ctl->GetClientWidth();

   // LPRECT lpRect;
   // ctlList->GetClientRect(lpRect);
   // if (tblwid < (lpRect->right - lpRect->left)) tblwid = (lpRect->right - lpRect->left);

   // tblwid = nWidth;// XTwipsToPixels((long)(tblwid * EXTRAWIDTH));
    

    // save table header
    strHdr.Format("<FONT FACE=\"%s\" SIZE=1>\n"
                  "<TABLE BORDER CELLSPACING=0 CELLPADDING=2 VALIGN=CENTER "
                  "BGCOLOR=%s "
                  "BORDERCOLOR=%s "
                  "WIDTH=%d>\n\n",
                  _T("Arial"),
                  HTMLColor(0x00FFFFFF),//Back Color
                  HTMLColor(0x00000000),//Grid Color
                  (int)tblwid);
    fprintf(f, (LPCSTR)strHdr);
    
    // loop through the rows
    long r, c;
    CRect lRect;
/////////////////////////////////////////////////////////
  

 
 
    CString strTxt;
    CString strTemp; 
    CString strBdr;
    CString strBkg;
    CString strSpan;
    CString strFfx;
    CString strCell;
    CString strAln;

    CString strFnt;
    double wid = 0;

    fprintf(f, "<TR>\n");

    //char* sHeader2[13] = { "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };
    //int  nWidth[13] = { 40, 140, 90, 70, 90, 75, 75, 65, 200, 200, 76, 500,10 }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};

  /*  strTemp = "Y";
    strTemp = HTMLText(strTemp);
  
    wid = 10;
    wid = XTwipsToPixels((long)(EXTRAWIDTH * wid));

    strTxt = "  <TD WIDTH = \"2 % \" BGCOLOR = \"#ffffff\" ALIGN = CENTER NOWRAP><FONT COLOR = \"#000000\">";
    strTxt += strTemp;
    strTxt += "< / FONT>< / TD>";
    strCell = strTxt;
    fprintf(f, "%s", strCell);<TD WIDTH="19%" BGCOLOR="#d2d8b0" NOWRAP><FONT COLOR="#336699">STEP</FONT></TD>*/

    strBkg = " BGCOLOR=" + HTMLColor(0xd2d8b0);
    strFnt = " COLOR = " + HTMLColor(0x336699);

    // var: strAln

    strAln = " ALIGN=CENTER";

    wid = 10;
    strTxt = "Y";// sHeader2[c];
    strCell.Format("<TD WIDTH=\"%d%%\"%s%s%s%s%s>",
        int((100 * wid) / tblwid), strBkg, strAln, strBdr, strSpan,
        " NOWRAP");
    if (strFnt.GetLength())
        strCell = strCell + CString("<FONT") + strFnt + CString(">");
    strCell = strCell + strFfx + strTxt;
    if (strFfx.Find("B") >= 0) strCell = strCell + "</B>";
    if (strFfx.Find("I") >= 0) strCell = strCell + "</I>";
    if (strFfx.Find("U") >= 0) strCell = strCell + "</U>";
    if (strFnt.GetLength())    strCell = strCell + "</FONT>";

    // end cell
    strCell = strCell + "</TD>\n";
    fprintf(f, "%s", strCell);


    for (c = 0; c < MAX_COLUMN_NO; c++)
    {
        strSpan = "";
        strTxt = "";
        strTemp = "";
        strBdr = "";
        strBkg = "";
        strSpan = "";
        strFfx = "";
        strCell = "";
        strAln = "";
        strFnt = "";

         wid = nWidth[c];
        strTxt = sHeader2[c]; 
        strBkg = " BGCOLOR=" + HTMLColor(0xd2d8b0);
        strFnt = " COLOR = " + HTMLColor(0x336699);

        strAln = " ALIGN=CENTER";


        strCell.Format("<TD WIDTH=\"%d%%\"%s%s%s%s%s>",
            int((100 * wid) / tblwid), strBkg, strAln, strBdr, strSpan,
            " NOWRAP");
        if (strFnt.GetLength())
            strCell = strCell + CString("<FONT") + strFnt + CString(">");
        strCell = strCell + strFfx + strTxt;
        if (strFfx.Find("B") >= 0) strCell = strCell + "</B>";
        if (strFfx.Find("I") >= 0) strCell = strCell + "</I>";
        if (strFfx.Find("U") >= 0) strCell = strCell + "</U>";
        if (strFnt.GetLength())    strCell = strCell + "</FONT>";

        // end cell
        strCell = strCell + "</TD>\n";
        fprintf(f, "%s", strCell);
    }

    // end row
    fprintf(f, "</TR>\n");
   

    for (r = 0; r < ctlList->GetItemCount(); r++) {
                
   
        // start row
        fprintf(f, "<TR>\n");
        
        strSpan = "";
        strTxt = "";
        strTemp = "";
        strBdr = "";
        strBkg = "";
        strSpan = "";
        strFfx = "";
        strCell = "";
        strAln = "";
        strFnt = "";

        wid = 10;
       // wid = XTwipsToPixels((long)(EXTRAWIDTH * wid));

   /*     CString strTxt;
        CString strTemp;*/
        if (TRUE == ctlList->GetCheck(r))
            strTemp = "Y";
        else
            strTemp = "";
        strTemp = HTMLText(strTemp);

        wid = 10;// nWidth[c];
        strTxt = strTemp;// sHeader2[c];
        strBkg = " BGCOLOR=" + HTMLColor(0xffffff);
        strFnt = " COLOR = " + HTMLColor(0x000000);

        strAln = " ALIGN=CENTER";


        strCell.Format("<TD WIDTH=\"%d%%\"%s%s%s%s%s>",
            int((100 * wid) / tblwid), strBkg, strAln, strBdr, strSpan,
            " NOWRAP");
        if (strFnt.GetLength())
            strCell = strCell + CString("<FONT") + strFnt + CString(">");
        strCell = strCell + strFfx + strTxt;
        if (strFfx.Find("B") >= 0) strCell = strCell + "</B>";
        if (strFfx.Find("I") >= 0) strCell = strCell + "</I>";
        if (strFfx.Find("U") >= 0) strCell = strCell + "</U>";
        if (strFnt.GetLength())    strCell = strCell + "</FONT>";
        strCell = strCell + "</TD>\n";
        //strCell = strTxt;
        fprintf(f, "%s", strCell);

  /*      <TD WIDTH = "2%" BGCOLOR = "#f7f7e7" ALIGN = CENTER NOWRAP><FONT COLOR = "#336699">Y< / FONT>< / TD>
            <TD WIDTH = "3%" BGCOLOR = "#d2d8b0" ALIGN = CENTER NOWRAP><FONT COLOR = "#336699">NO< / FONT>< / TD>
 */   

        // loop through the columns
        for (c = 0; c < MAX_COLUMN_NO; c++) {
            
          
            wid = nWidth[c];                    
            strTxt = ctlList->GetItemText(r,c); //V_BSTR(&vCell);      
            strTxt = HTMLText(strTxt);
            
           
            if (c == 2)
            {
               // <TD WIDTH = "7%" BGCOLOR = "#ff0000" ALIGN = CENTER NOWRAP><FONT COLOR = "#ffffff">NG< / FONT>< / TD>
                if (strTxt.Find("NG") >= 0)
                {
                    strBkg = " BGCOLOR=" + HTMLColor(0x0000ff);
                    
                    strFnt = "COLOR = "  + HTMLColor(0xffffff);
                }
                else if (strTxt.Find("OK") >= 0)
                {
                    strBkg = " BGCOLOR=" + HTMLColor(0xff0000);
                    strFnt = "COLOR = " + HTMLColor(0xffffff);
                }
           
            } 
            else
                {
                    strBkg = " BGCOLOR=" + HTMLColor(0xffffff);
                    strFnt = " COLOR = " + HTMLColor(0x000000);
                }
           
            switch (c) {
               
                case 0:
                case 2:
                    strAln = " ALIGN=CENTER";
                    break;
                case 1:
                    strAln = " ALIGN=LEFT";
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                    strAln = " ALIGN=RIGHT";
                    break;
                default:
                    strAln = " ALIGN=CENTER";
                    break;
                
            }

            // build HTML cell string
			//
			// REVIEW:
			// we honor the WordWrap property by adding a NOWRAP tag to the row.
			// but NOWRAP in HTML does not trunctae the contents, it just 
			// widens the column until the whole thing fits.
			//
            CString strCell;
            strCell.Format("<TD WIDTH=\"%d%%\"%s%s%s%s%s>",
                int((100 * wid) / tblwid), strBkg, strAln, strBdr, strSpan,
				 " NOWRAP");
            if (strFnt.GetLength())
                strCell = strCell + CString("<FONT") + strFnt + CString(">");
            strCell = strCell + strFfx + strTxt;
            if (strFfx.Find("B") >= 0) strCell = strCell + "</B>";
            if (strFfx.Find("I") >= 0) strCell = strCell + "</I>";
            if (strFfx.Find("U") >= 0) strCell = strCell + "</U>";
            if (strFnt.GetLength())    strCell = strCell + "</FONT>";
            
            // end cell
            strCell = strCell + "</TD>\n";
            fprintf(f, "%s", strCell);
        }
        
        // end row
        fprintf(f, "</TR>\n");
    }
    
    // save table end
    strHdr = "</TABLE>\n\n</BODY>\n</HTML>\n";
    fprintf(f, strHdr);
    
    // close file
    fclose(f);
    
    // all done
    return TRUE;
}
BOOL SaveGridToText(CListCtrl* ctlList, LPSTR fname)
{
  //  char* sHeader2[14] = { "NO","STEP", "RESULT","T/T", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };
  //  int  nWidth[14] = { 40, 100, 90,40, 70, 90, 75, 75, 65, 200, 200, 76, 500,10 }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};

    // create a dummy variant we can use when we want to omit optional parameters
  

    // additional width for HTML columns
    double EXTRAWIDTH = 1.3;

    // open the file, quit on failure
    FILE* f = fopen(fname, "wt");
    if (!f) return FALSE;

 
    CString strTxt;
    CString strCell;


    CString strFnt;
    double wid = 0;


    strTxt = "Y NO STEP    RESULT  T/T  MEAS    TARGET  L-LIMIT U-LIMIT UNIT    A/T(FREQ,LVL)   A/M(FREQ,LVL)   TIME    MESSAGE";
    strCell = strTxt+"\n";
    fprintf(f, "%s", strCell);

           


    for (int r = 0; r < ctlList->GetItemCount(); r++) {


        // start row     
     
        strTxt = "";      
        strCell = "";
        strFnt = "";

        wid = 10;
        // wid = XTwipsToPixels((long)(EXTRAWIDTH * wid));

    /*     CString strTxt;
         CString strTemp;*/
        if (TRUE == ctlList->GetCheck(r))
            strTxt = "Y";
        else
            strTxt = " ";
        strTxt += "    ";      
        strCell = strCell + strTxt;    
    

       // loop through the columns
        for (int c = 0; c < MAX_COLUMN_NO; c++) 
        {                            
            strTxt = ctlList->GetItemText(r, c); //V_BSTR(&vCell);      
            strTxt += "    ";        

            strCell = strCell + strTxt;
            
        }
        strCell += "\n";

        // end row
          fprintf(f, "%s", strCell);
    }

    // save table end
 

    // close file
    fclose(f);

    return TRUE;
}
BOOL SaveGridToCSV(CListCtrl* ctlList, LPSTR fname)
{
   // char* sHeader2[13] = { "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };
   // int  nWidth[13] = { 40, 140, 90, 70, 90, 75, 75, 65, 200, 200, 76, 500,10 }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};

    // create a dummy variant we can use when we want to omit optional parameters



    // open the file, quit on failure
    FILE* f = fopen(fname, "wt");
    if (!f) return FALSE;


    CString strTxt;
    CString strCell;


    CString strFnt;
    double wid = 0;


    strTxt = "Y,NO,STEP,RESULT,T/T,MEAS,TARGET,L-LIMIT,U-LIMIT,UNIT,A/T(FREQ LVL),A/M(FREQ LVL),TIME,MESSAGE";
    strCell = strTxt + "\n";
    fprintf(f, "%s", strCell);


    for (int r = 0; r < ctlList->GetItemCount(); r++)
    {        
        strTxt = "";
        strCell = "";
        strFnt = "";

        if (TRUE == ctlList->GetCheck(r))
            strTxt = "Y";
        else
            strTxt = " ";
    
        strCell = strCell + strTxt;


        // loop through the columns
        for (int c = 0; c < MAX_COLUMN_NO; c++)
        {
             strTxt = ",";        
             strTxt += ctlList->GetItemText(r, c); //V_BSTR(&vCell);    
             strCell = strCell + strTxt;

        }
        strCell += "\n";

        // end row
        fprintf(f, "%s", strCell);
    }

    fclose(f);

    return TRUE;
}

#else
BOOL SaveGridToHTML(CVSFlexGrid* ctl, LPSTR fname)
{
    // create a dummy variant we can use when we want to omit optional parameters
    VARIANT vDefVal;
    V_VT(&vDefVal) = VT_ERROR;

    // variant to use as scratch
    COleVariant vCell;

    // additional width for HTML columns
    double EXTRAWIDTH = 1.3;

    // open the file, quit on failure
    FILE* f = fopen(fname, "wt");
    if (!f) return FALSE;

    // save page header
    CString strHdr;
    strHdr = "<HTML>\n<HEAD>\n"
        "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html;charset=windows-1252\">\n"
        "<META NAME=\"Generator\" CONTENT=\"VSFlexGrid Pro\">\n"
        "<TITLE>VSFlexGrid Pro</TITLE>\n"
        "</HEAD>\n<BODY>\n\n";
    fprintf(f, (LPCSTR)strHdr);

    // get total table width in pixels
    double tblwid = 0;
    for (int i = 0; i < ctl->GetCols() - 1; i++)
        tblwid += ctl->GetColWidth(i);
    if (tblwid < ctl->GetClientWidth()) tblwid = ctl->GetClientWidth();
    tblwid = XTwipsToPixels((long)(tblwid * EXTRAWIDTH));

    // save table header
    strHdr.Format("<FONT FACE=\"%s\" SIZE=1>\n"
        "<TABLE BORDER CELLSPACING=0 CELLPADDING=2 VALIGN=CENTER "
        "BGCOLOR=%s "
        "BORDERCOLOR=%s "
        "WIDTH=%d>\n\n",
        ctl->GetFontName(),
        HTMLColor(ctl->GetBackColor()),
        HTMLColor(ctl->GetGridColor()),
        (int)tblwid);
    fprintf(f, (LPCSTR)strHdr);

    // loop through the rows
    long r, c;
    for (r = 0; r < ctl->GetRows(); r++) {

        // skip hidden rows
        if (ctl->GetRowHidden(r) || !ctl->GetRowHeight(r)) continue;

        // start row
        fprintf(f, "<TR>\n");

        // loop through the columns
        for (c = 0; c < ctl->GetCols(); c++) {

            // skip hidden cols
            if (ctl->GetColHidden(c) || !ctl->GetColWidth(c)) continue;

            // handle merges
            // var: strSpan
            CString strSpan;
            long r1, c1, r2, c2;
            ctl->GetMergedRange(r, c, &r1, &c1, &r2, &c2);
            if (c1 < c || r1 < r) continue;
            if (c2 > c) strSpan.Format(" COLSPAN=%d", c2 - c + 1);
            if (r2 > r) strSpan.Format(" ROWSPAN=%d", r2 - r + 1);

            // get col width
            // var: wid
            double wid = 0;
            for (int i = c1; i <= c2; i++)
                wid += ctl->GetColWidth(i);
            wid = XTwipsToPixels((long)(EXTRAWIDTH * wid));

            // get cell text
            // var: strTxt
            vCell = ctl->GetCell(flexcpTextDisplay, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            CString strTxt = V_BSTR(&vCell);
            if (r >= ctl->GetFixedRows() && ctl->GetColDataType(c) == flexDTBoolean)
                strTxt = atoi((LPCSTR)strTxt) ? "Y" : "";
            strTxt = HTMLText(strTxt);

            // get outline indent
            // var: strIndent
            static CString strIndent;
            if (ctl->GetOutlineBar() > 0 && c == ctl->GetOutlineCol()) {
                if (ctl->GetIsSubtotal(r)) {
                    strIndent = "";
                    for (int i = 0; i < ctl->GetRowOutlineLevel(r); i++)
                        strIndent = strIndent + "&nbsp&nbsp&nbsp&nbsp";
                    strTxt = strIndent + strTxt;
                    strIndent = strIndent + "&nbsp&nbsp&nbsp&nbsp";
                }
                else {
                    strTxt = strIndent + strTxt;
                }
            }

            // get back color
            // var: strBkg
            CString strBkg;
            vCell = ctl->GetCell(flexcpBackColor, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            vCell.ChangeType(VT_I4);
            long clr = V_I4(&vCell);
            if (clr) {
                strBkg = " BGCOLOR=" + HTMLColor(clr);
            }
            else if (r < ctl->GetFixedRows() || c < ctl->GetFixedCols()) {
                strBkg = " BGCOLOR=" + HTMLColor(ctl->GetBackColorFixed());
            }

            // get border color
            // var: strBdr
            CString strBdr;
            if (r < ctl->GetFixedRows() || c < ctl->GetFixedCols())
                strBdr = " BORDERCOLOR=" + HTMLColor(ctl->GetGridColorFixed());

            // get fore color and font name
            // var: strFnt
            CString strFnt;
            vCell = ctl->GetCell(flexcpFontName, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            CString strFontName = V_BSTR(&vCell);
            if (strFontName.GetLength() && strFontName != ctl->GetFontName())
                strFnt = " FACE=\"" + strFnt + "\"";
            vCell = ctl->GetCell(flexcpForeColor, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            vCell.ChangeType(VT_I4);
            if (V_I4(&vCell) && V_I4(&vCell) != (long)ctl->GetForeColor()) {
                strFnt = strFnt + CString(" COLOR=") + HTMLColor(V_I4(&vCell));
            }
            else if (r < ctl->GetFixedRows() || c < ctl->GetFixedCols()) {
                strFnt = strFnt + CString(" COLOR=") + HTMLColor(ctl->GetForeColorFixed());
            }

            // get font effects
            // var: strFfx
            CString strFfx;
            vCell = ctl->GetCell(flexcpFontBold, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            vCell.ChangeType(VT_I2);
            if (V_I2(&vCell)) strFfx = strFfx + "<B>";
            vCell = ctl->GetCell(flexcpFontItalic, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            vCell.ChangeType(VT_I2);
            if (V_I2(&vCell)) strFfx = strFfx + "<I>";
            vCell = ctl->GetCell(flexcpFontUnderline, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
            vCell.ChangeType(VT_I2);
            if (V_I2(&vCell)) strFfx = strFfx + "<U>";

            // get alignment
            // var: strAln
            CString strAln;
            switch (ctl->GetColAlignment(c)) {
            case flexAlignCenterBottom:
                strAln = " ALIGN=CENTER VALIGN=BOTTOM";
                break;
            case flexAlignCenterCenter:
                strAln = " ALIGN=CENTER";
                break;
            case flexAlignCenterTop:
                strAln = " ALIGN=CENTER VALIGN=TOP";
                break;
            case flexAlignLeftBottom:
                strAln = " VALIGN=BOTTOM";
                break;
            case flexAlignLeftCenter:
                strAln = "";
                break;
            case flexAlignLeftTop:
                strAln = " VALIGN=TOP";
                break;
            case flexAlignRightBottom:
                strAln = " ALIGN=RIGHT VALIGN=BOTTOM";
                break;
            case flexAlignRightCenter:
                strAln = " ALIGN=RIGHT";
                break;
            case flexAlignRightTop:
                strAln = " ALIGN=RIGHT VALIGN=TOP";
                break;
            default:
                switch (ctl->GetColDataType(c)) {
                case flexDTBoolean:
                    strAln = " ALIGN=CENTER";
                    break;
                case flexDTDate:
                    strAln = "";
                    break;
                default:
                    vCell = ctl->GetCell(flexcpTextDisplay, COleVariant(r), COleVariant(c), vDefVal, vDefVal);
                    strAln = V_BSTR(&vCell);
                    strAln = (IsNumber((LPCSTR)strAln)) ? " ALIGN=RIGHT" : "";
                }
            }

            // build HTML cell string
            //
            // REVIEW:
            // we honor the WordWrap property by adding a NOWRAP tag to the row.
            // but NOWRAP in HTML does not trunctae the contents, it just 
            // widens the column until the whole thing fits.
            //
            CString strCell;
            strCell.Format("<TD WIDTH=\"%d%%\"%s%s%s%s%s>",
                int((100 * wid) / tblwid), strBkg, strAln, strBdr, strSpan,
                ctl->GetWordWrap() ? "" : " NOWRAP");
            if (strFnt.GetLength())
                strCell = strCell + CString("<FONT") + strFnt + CString(">");
            strCell = strCell + strFfx + strTxt;
            if (strFfx.Find("B") >= 0) strCell = strCell + "</B>";
            if (strFfx.Find("I") >= 0) strCell = strCell + "</I>";
            if (strFfx.Find("U") >= 0) strCell = strCell + "</U>";
            if (strFnt.GetLength())    strCell = strCell + "</FONT>";

            // end cell
            strCell = strCell + "</TD>\n";
            fprintf(f, "%s", strCell);
        }

        // end row
        fprintf(f, "</TR>\n");
    }

    // save table end
    strHdr = "</TABLE>\n\n</BODY>\n</HTML>\n";
    fprintf(f, strHdr);

    // close file
    fclose(f);

    // all done
    return TRUE;
}

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ControlToBMP(HWND hwnd) 
//
// this function copies a VideoSoft control to a bitmap.
// it has been tested with VSFlexGrid and VSPrinter.
// it may or may not work with other ActiveX controls.
// the caller is responsible for freeing the bitmap.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HBITMAP ControlToBMP(HWND hWnd)
{
	// create temporary dc for drawing into
    HDC hdcScrn = GetDC(hWnd);
    HDC hdcTemp = CreateCompatibleDC(hdcScrn);

	// create bitmap and attach to temporary DC
	RECT rc;
	GetClientRect(hWnd, &rc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScrn, rc.right, rc.bottom);
	HBITMAP holdbmp = (HBITMAP)SelectObject(hdcTemp, hbmp);

	// tell control to draw itself into temporary dc
	SendMessage(hWnd, WM_PAINT, (WPARAM)hdcTemp, 0L);
	DrawEdge(hdcTemp, &rc, EDGE_SUNKEN, BF_RECT);

	// get rid of the temporary dc (keep bitmap)
    ReleaseDC(hWnd, hdcScrn);
	SelectObject(hdcTemp, holdbmp);
	DeleteDC(hdcTemp);

	// return the bitmap: it is the caller's responsibility to release it
	return hbmp;
}
