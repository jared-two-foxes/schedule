#ifndef GOOGLEAPIS_SHEETS_HPP__
#define GOOGLEAPIS_SHEETS_HPP__

namespace googleapis {
namespace sheets {

/*
 * An overview...
 *
 * So basically to get the values of a range in a sheet we go
 * GET https://sheets.googleapis.com/v4/spreadsheets/{spreadsheetId}/values/{range}
 *
 * where spreadsheetId is the ID of the spreadsheet to retrieve data from and range
 * is the A1 notation of the values to retrieve.
 *
 * Theres also a bunch of query parameters that we can also request [majorDimension,
 * valueRenderOption, dateTimeRenderOption] which affect how the returned data is
 * presented
 *
 * The response body will contain an instance of a ValueRange object which looks
 * like:
 * {
 *    "range": string,
 *    "majorDimension": enum(Dimension)
 *    "values": [Value]
 * }
 *
 * Where the Value type is basically a union which looks like the following.
 * union Value {
 *   NulLValue null_value;
 *   double number_value;
 *   string string_value;
 *   bool bool_value;
 *   Struct struct_value;
 *   ListValue list_value;
 * }
 *
 * Doesnt quite translate, obviously Struct and ListValues dont quite map to anything
 * useful, I'm assuming Struct is just some abstract complex type while ListValue
 * is an array of ListValues.
 *
 * OR we can go
 * GET https://sheets.googleapis.com/v4/spreadsheets/{spreadsheetId}
 *
 * The response body of which returns a Spreadsheet instance.

//Spreadsheet
{
  "spreadsheetId": string,
  "properties": {
    object(SpreadsheetProperties)
  },
  "sheets": [{
    object(Sheet)
  }],
  "namedRanges":[{
    object(NamedRange)
  }],
  "spreadsheetUrl": string,
  "developerMetadata": [stuff]
}


//Sheet
{
  "properties": SheetProperties,
  "data": [GridData],
  "merges": [GridRange],
  "conditionalFormats": [ConditionalFormatRule],
  "filterViews": [FilterView]
  "protectedRanges": [ProtectedRange],
  "basicFilter": [BasicFilter],
  "charts": [EmbeddedChart]
  "bandedRanges": [BandedRange],
  "developerMetadata": [DeveloperMetdata]
  "rowGroups": [DimensionGroup],
  "columnGroups": [DimensionGroup],
}

//GridData
{
  "startRow": number
  "startColumn": number
  "rowData": [RpwData]
  "rowMetadata": [DimensionProperties,.
  "columnMetadata": [DimensionProperties]
}

//RowData
{
  "values": [CellData]
}

//CellData
{
  "userEnteredValue": ExtendedValue
  "effectivedValue": ExtendedValue
  "formatValue": string
  "userEnteredFormat": CellFormat
  "effectiveFormat": CellFormat
  "hyperlink": string
  "note": string
  "textFormatRuns": TExtFormatRun
  "dataValidation": DataValidationRule
  "pivotTable": PivotTable
}

// ExtendedValue, can only be one of the following
{
  "numberValue": number
  "stringValue": string
  "boolValue"" bool
  "formulaValue": string
  "errorValue": ErrorValue
}

// ErrorValue
{
  "type": enum(ErrorType)
  "message": string
}



 *
 */


}
}

#endif // GOOGLEAPIS_SHEETS_HPP__
