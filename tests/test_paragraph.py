import skia
import pytest
import operator

@pytest.fixture(scope='session')
def textlayout_font_collection():
    return skia.textlayout.FontCollection()

def test_FontCollection_init0(textlayout_font_collection):
    assert isinstance(textlayout_font_collection, skia.textlayout_FontCollection)


@pytest.fixture(scope='session')
def paragraph_style():
    return skia.textlayout.ParagraphStyle()

def test_ParagraphStyle_init0(paragraph_style):
    assert isinstance(paragraph_style, skia.textlayout_ParagraphStyle)


@pytest.fixture(scope='session')
def strut_style():
    return skia.textlayout.StrutStyle()

def test_StrutStyle_init0(strut_style):
    assert isinstance(strut_style, skia.textlayout_StrutStyle)


@pytest.fixture(scope='session')
def textlayout_text_style():
    return skia.textlayout.TextStyle()

def test_TextStyle_init0(textlayout_text_style):
    assert isinstance(textlayout_text_style, skia.textlayout_TextStyle)


@pytest.fixture(scope='session')
def paragraph_builder():
    return skia.textlayout.ParagraphBuilder.make(skia.textlayout.ParagraphStyle(),
                                                 skia.textlayout.FontCollection(),
                                                 skia.Unicodes.ICU.Make())

def test_ParagraphBuilder_init0(paragraph_builder):
    assert isinstance(paragraph_builder, skia.textlayout_ParagraphBuilder)

def test_Paragraph_init0(paragraph_builder):
    paragraph_builder.addText("")
    paragraph = paragraph_builder.Build()
    assert isinstance(paragraph, skia.textlayout_Paragraph)


# Adapted from #278, to make sure that "\n" results in a line break  (rather that .notdef).
# Height is larger than twice width, if a break happens.
def test_Paragraph_linebreak(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_text_style.setFontSize(50)
    textlayout_text_style.setForegroundPaint(paint)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    builder = skia.textlayout.ParagraphBuilder.make(
        paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
    )
    builder.pushStyle(textlayout_text_style)

    builder.addText("o\no")
    paragraph = builder.Build()
    paragraph.layout(300)
    assert (paragraph.Height > 0) and (paragraph.Height > paragraph.LongestLine * 2)


@pytest.mark.parametrize('test_operator, spec_a, spec_b', [
    (operator.eq, (False, 1.0), (True, 1.0)),
    (operator.eq, (True, 0), (True, 1.0)),
    (operator.eq, (True, 0.5), (True, 1.0)),
    (operator.lt, (True, 1.0), (True, 2.0)),
    (operator.lt, (True, 2.0), (True, 3.0)),
])
def test_Paragraph_strutHeight(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style, test_operator, strut_style, spec_a, spec_b):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_text_style.setFontSize(50)
    textlayout_text_style.setForegroundPaint(paint)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    def graf_with_strut(enabled, leading_factor):
        strut_style.setStrutEnabled(enabled)
        strut_style.setLeading(leading_factor)
        paragraph_style.setStrutStyle(strut_style)

        builder = skia.textlayout.ParagraphBuilder.make(
            paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
        )
        builder.pushStyle(textlayout_text_style)

        builder.addText("o\no")
        paragraph = builder.Build()
        paragraph.layout(300)

        return paragraph
    
    paragraph_a_height = graf_with_strut(*spec_a).Height
    paragraph_b_height = graf_with_strut(*spec_b).Height

    assert test_operator(paragraph_a_height, paragraph_b_height)


@pytest.mark.parametrize('spacing_a, spacing_b', [
    (-1, 0),
    (0, 1),
    (1, 2),
    (2, 3),
    (-1, 1),
])
def test_Paragraph_letterSpacing(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style, strut_style, spacing_a, spacing_b):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    def graf_with_letterspacing(letterspacing):
        textlayout_text_style.setFontSize(50)
        textlayout_text_style.setForegroundPaint(paint)
        textlayout_text_style.setLetterSpacing(letterspacing)

        builder = skia.textlayout.ParagraphBuilder.make(
            paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
        )
        builder.pushStyle(textlayout_text_style)

        builder.addText("ooo")
        paragraph = builder.Build()
        paragraph.layout(300)

        return paragraph
    
    assert graf_with_letterspacing(spacing_a).LongestLine < graf_with_letterspacing(spacing_b).LongestLine


@pytest.mark.parametrize('spacing_a, spacing_b', [
    (-1, 0),
    (0, 1),
    (1, 2),
    (2, 3),
    (-1, 1),
])
def test_Paragraph_wordSpacing(paragraph_builder, textlayout_text_style, textlayout_font_collection, paragraph_style, strut_style, spacing_a, spacing_b):
    paint = skia.Paint()
    paint.setColor(skia.ColorBLACK)
    paint.setAntiAlias(True)

    textlayout_font_collection.setDefaultFontManager(skia.FontMgr())

    def graf_with_word_spacing(letterspacing):
        textlayout_text_style.setFontSize(50)
        textlayout_text_style.setForegroundPaint(paint)
        textlayout_text_style.setWordSpacing(letterspacing)

        builder = skia.textlayout.ParagraphBuilder.make(
            paragraph_style, textlayout_font_collection, skia.Unicodes.ICU.Make()
        )
        builder.pushStyle(textlayout_text_style)

        builder.addText("word word word")
        paragraph = builder.Build()
        paragraph.layout(300)

        return paragraph
    
    assert graf_with_word_spacing(spacing_a).LongestLine < graf_with_word_spacing(spacing_b).LongestLine