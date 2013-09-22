Option Explicit

function count(find as string, lookin as range) As Long
   dim cell As Range
   for each cell in lookin
       if (cell.Value = find) then count = count + 1 '//case sens
   next
end function

Public Sub Columns_to_TextFile()
     
    Const My_Path = "C:\TEXTFILES\"
    Dim iCol As Integer
    Dim lRow As Long
    Dim File_Num As Long
    Dim NewPath As String
         
    On Error Resume Next
    If Trim(Dir(My_Path, vbDirectory)) = "" Then
        MkDir My_Path
    Else
        Kill My_Path & "*.txt"
    End If
    On Error GoTo 0
    File_Num = FreeFile()
    With ActiveSheet
        For iCol = 1 To COUNTA(A2:A2000)
            NewPath = My_Path & Trim(.Cells(1, iCol).Value) & ".txt"
            Open NewPath For Output As #File_Num
            For lRow = 2 To .Rows.Count
                Print #File_Num, .Cells(lRow, iCol).Value
            Next
            Close #File_Num
        Next
    End With
     
    MsgBox "All files created and saved to : " & My_Path
     
End Sub

