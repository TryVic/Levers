object fm: Tfm
  Left = 422
  Top = 222
  Width = 800
  Height = 600
  Caption = 'Кинематическая модель'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object pnImage: TPanel
    Left = 133
    Top = 0
    Width = 659
    Height = 566
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object sbX: TScrollBar
      Left = 0
      Top = 550
      Width = 659
      Height = 16
      Align = alBottom
      LargeChange = 50
      Max = 1000
      Min = -1000
      PageSize = 100
      SmallChange = 10
      TabOrder = 0
      OnChange = sbXChange
    end
    object sbY: TScrollBar
      Left = 642
      Top = 0
      Width = 17
      Height = 550
      Align = alRight
      Kind = sbVertical
      LargeChange = 50
      Max = 1000
      Min = -1000
      PageSize = 100
      SmallChange = 10
      TabOrder = 1
      OnChange = sbXChange
    end
    object pn: TPanel
      Left = 0
      Top = 0
      Width = 642
      Height = 550
      Align = alClient
      Caption = 'Некорректные параметры системы.'
      TabOrder = 2
      object image: TImage
        Left = 1
        Top = 1
        Width = 640
        Height = 548
        Align = alClient
      end
    end
  end
  object pnControl: TPanel
    Left = 0
    Top = 0
    Width = 133
    Height = 566
    Align = alLeft
    TabOrder = 1
    object gbParam: TGroupBox
      Left = 1
      Top = 82
      Width = 131
      Height = 216
      Align = alTop
      Caption = 'Параметры'
      TabOrder = 0
    end
    object gbAngle: TGroupBox
      Left = 1
      Top = 1
      Width = 131
      Height = 81
      Align = alTop
      Caption = 'Угол'
      TabOrder = 1
      object lbHint: TLabel
        Left = 25
        Top = 18
        Width = 43
        Height = 13
        Alignment = taRightJustify
        Caption = 'текущий'
      end
      object lbSpeed: TLabel
        Left = 21
        Top = 39
        Width = 47
        Height = 13
        Alignment = taRightJustify
        Caption = 'скорость'
      end
      object lbAngleSpeed: TLabel
        Left = 72
        Top = 39
        Width = 3
        Height = 13
        Caption = '-'
      end
      object edAngle: TEdit
        Left = 72
        Top = 15
        Width = 40
        Height = 21
        TabOrder = 0
        Text = '0'
      end
      object udAngle: TUpDown
        Left = 112
        Top = 15
        Width = 15
        Height = 21
        Associate = edAngle
        Min = -1
        Max = 360
        Position = 0
        TabOrder = 1
        Wrap = False
        OnClick = udAngleClick
      end
      object tbAngleSpeed: TTrackBar
        Left = 2
        Top = 54
        Width = 127
        Height = 20
        Max = 20
        Min = 1
        Orientation = trHorizontal
        PageSize = 1
        Frequency = 1
        Position = 3
        SelEnd = 0
        SelStart = 0
        TabOrder = 2
        ThumbLength = 10
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = tbAngleSpeedChange
      end
    end
    object gbView: TGroupBox
      Left = 1
      Top = 298
      Width = 131
      Height = 129
      Align = alTop
      Caption = 'Вид'
      TabOrder = 2
      object lbScale: TLabel
        Left = 6
        Top = 102
        Width = 46
        Height = 13
        Caption = 'Масштаб'
      end
      object cbAnimation: TCheckBox
        Left = 6
        Top = 15
        Width = 76
        Height = 17
        Caption = 'Анимация'
        TabOrder = 0
        OnClick = cbAnimationClick
      end
      object cbSize: TCheckBox
        Left = 6
        Top = 30
        Width = 76
        Height = 17
        Caption = 'Размеры'
        TabOrder = 1
        OnClick = cbSizeClick
      end
      object cbAngle: TCheckBox
        Left = 6
        Top = 45
        Width = 76
        Height = 17
        Caption = 'Углы'
        TabOrder = 2
        OnClick = cbAngleClick
      end
      object cbText: TCheckBox
        Left = 6
        Top = 60
        Width = 88
        Height = 17
        Caption = 'Обозначения'
        TabOrder = 3
        OnClick = cbTextClick
      end
      object cbScale: TComboBox
        Left = 66
        Top = 99
        Width = 58
        Height = 22
        Style = csOwnerDrawFixed
        ItemHeight = 16
        TabOrder = 4
        OnChange = cbScaleChange
        Items.Strings = (
          '25'
          '50'
          '80'
          '90'
          '100'
          '150'
          '200'
          '300')
      end
      object cbForce: TCheckBox
        Left = 6
        Top = 75
        Width = 121
        Height = 17
        Caption = 'Силы (180 об/мин)'
        TabOrder = 5
        OnClick = cbForceClick
      end
    end
    object gbWindows: TGroupBox
      Left = 1
      Top = 427
      Width = 131
      Height = 105
      Align = alTop
      Caption = 'Окна'
      TabOrder = 3
      object btGraphics: TButton
        Left = 6
        Top = 15
        Width = 118
        Height = 22
        Caption = 'Графики'
        TabOrder = 0
        OnClick = btGraphicsClick
      end
      object Button1: TButton
        Left = 6
        Top = 39
        Width = 118
        Height = 22
        Caption = 'Динамика'
        TabOrder = 1
        OnClick = Button1Click
      end
    end
  end
  object Timer: TTimer
    Enabled = False
    Interval = 10
    OnTimer = TimerTimer
    Left = 142
    Top = 9
  end
end
