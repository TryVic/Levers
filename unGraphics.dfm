object fmGraphics: TfmGraphics
  Left = 246
  Top = 232
  Width = 715
  Height = 536
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Графики'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Image: TImage
    Left = 79
    Top = 25
    Width = 628
    Height = 477
    Align = alClient
  end
  object pnControl: TPanel
    Left = 0
    Top = 0
    Width = 707
    Height = 25
    Align = alTop
    TabOrder = 0
    object cbAuto: TCheckBox
      Left = 3
      Top = 3
      Width = 184
      Height = 17
      Caption = 'Автоматически пересчитывать'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = cbAutoClick
    end
    object btRecalc: TButton
      Left = 186
      Top = 3
      Width = 70
      Height = 19
      Caption = 'Обновить'
      TabOrder = 1
      Visible = False
      OnClick = btRecalcClick
    end
  end
  object pnButton: TPanel
    Left = 0
    Top = 25
    Width = 79
    Height = 477
    Align = alLeft
    BevelInner = bvLowered
    TabOrder = 1
  end
end
